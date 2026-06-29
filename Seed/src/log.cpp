#include "src/log.h"
#include <sys/stat.h>
#include <cerrno>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <ctime>
#include <unistd.h>
#include <pwd.h>
#include <execinfo.h>

namespace Seed {

static std::string GetConfigDir() {
    const char *xdg = getenv("XDG_CONFIG_HOME");
    std::string dir;
    if (xdg && xdg[0]) {
        dir = xdg;
    } else {
        const char *home = getenv("HOME");
        if (!home) {
            struct passwd *pw = getpwuid(getuid());
            home = pw->pw_dir;
        }
        dir = std::string(home) + "/.config";
    }
    return dir + "/Seed_Engine";
}

static bool CreateDirs(const std::string &path) {
    size_t pos = 0;
    while ((pos = path.find_first_of('/', pos + 1)) != std::string::npos) {
        mkdir(path.substr(0, pos).c_str(), 0755);
    }
    return mkdir(path.c_str(), 0755) == 0 || errno == EEXIST;
}

static const char *LevelName(LogLevel level) {
    switch (level) {
    case LogLevel::Trace:
        return "TRACE";
    case LogLevel::Debug:
        return "DEBUG";
    case LogLevel::Info:
        return "INFO ";
    case LogLevel::Warn:
        return "WARN ";
    case LogLevel::Error:
        return "ERROR";
    case LogLevel::Fatal:
        return "FATAL";
    }
    return "0";
}

static const char *LevelAnsiColor(LogLevel level) {
    switch (level) {
    case LogLevel::Trace:
        return "90";
    case LogLevel::Debug:
        return "36";
    case LogLevel::Info:
        return "32";
    case LogLevel::Warn:
        return "33";
    case LogLevel::Error:
        return "31";
    case LogLevel::Fatal:
        return "91";
    }
    return "0";
}

Logger &Logger::Get() {
    static Logger instance;
    return instance;
}

Logger::Logger() {
    std::string dir = GetConfigDir();
    CreateDirs(dir);

    std::string path = dir + "/log";
    m_file = fopen(path.c_str(), "w");
    if (!m_file) {
        fprintf(stderr, "\033[33m[WARN] Logger: could not open log file: %s\033[0m\n",
                path.c_str());
    }

    m_lastFlush = std::chrono::steady_clock::now();
    m_thread = std::thread(&Logger::ThreadFunc, this);
}

Logger::~Logger() { Shutdown(); }

void Logger::SetLevel(LogLevel level) { m_level = level; }

void Logger::SetQuiet(bool quiet) { m_quiet = quiet; }

void Logger::Shutdown() {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_running = false;
    }
    m_cv.notify_one();
    if (m_thread.joinable()) {
        m_thread.join();
    }
    if (m_file) {
        fclose(m_file);
        m_file = nullptr;
    }
}

void Logger::FlSync() {
    std::promise<void> promise;
    auto future = promise.get_future();
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_flReq.push(std::move(promise));
    }
    m_cv.notify_one();
    future.wait();
}

std::string Logger::FormatPrefix(LogLevel level, std::source_location loc) {
    auto now = std::chrono::system_clock::now();
    auto ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() %
        1000;
    auto t = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_r(&t, &tm);

    char ts[48];
    std::strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", &tm);

    char prefix[1152];
    int n = snprintf(prefix, sizeof(prefix), "[%s.%03lld] [%s] %s:%u %s -", ts, (long long)ms,
                     LevelName(level), loc.file_name(), loc.line(), loc.function_name());
    if (n < 0)
        return "[timestamp] [?????] ??? -";
    return std::string(prefix);
}

void Logger::Log(LogLevel level, std::source_location loc, const char *fmt, ...) {
    if (level < m_level)
        return;

    char msgBuf[4096];
    va_list args;
    va_start(args, fmt);
    int needed = vsnprintf(msgBuf, sizeof(msgBuf), fmt, args);
    va_end(args);

    std::string message;
    if (needed >= (int)sizeof(msgBuf)) {
        message.resize(needed + 1);
        va_list args2;
        va_start(args2, fmt);
        vsnprintf(message.data(), message.size(), fmt, args2);
        va_end(args2);
    } else {
        message = msgBuf;
    }

    std::string prefix = FormatPrefix(level, loc);
    std::string fullLine = prefix + " " + message;

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push({fullLine + "\n"});
    }
    m_cv.notify_one();

    if (!m_quiet) {
        fprintf(stderr, "\033[%sm%s\033[0m\n", LevelAnsiColor(level), fullLine.c_str());
    }
}

void Logger::ThreadFunc() {
    pthread_setname_np(pthread_self(), "Seed-Log");

    auto timeout = std::chrono::milliseconds(100);

    while (true) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait_for(lock, timeout,
                      [this]() { return !m_running || !m_queue.empty() || !m_flReq.empty(); });

        if (!m_running && m_queue.empty() && m_flReq.empty())
            break;

        std::queue<LogMessage> localMsgs;
        std::swap(localMsgs, m_queue);
        std::queue<std::promise<void>> localFlush;
        std::swap(localFlush, m_flReq);
        lock.unlock();

        while (!localMsgs.empty()) {
            m_batchBuffer += localMsgs.front().text;
            localMsgs.pop();
        }

        auto now = std::chrono::steady_clock::now();
        bool doFlush = (m_batchBuffer.size() >= kBatchSize) || ((now - m_lastFlush) >= timeout) ||
                       !localFlush.empty();

        if (doFlush) {
            if (m_file && !m_batchBuffer.empty()) {
                fwrite(m_batchBuffer.data(), 1, m_batchBuffer.size(), m_file);
                fflush(m_file);
            }
            m_batchBuffer.clear();
            m_lastFlush = now;

            while (!localFlush.empty()) {
                localFlush.front().set_value();
                localFlush.pop();
            }
        }
    }

    if (m_file && !m_batchBuffer.empty()) {
        fwrite(m_batchBuffer.data(), 1, m_batchBuffer.size(), m_file);
        fflush(m_file);
    }
}

} // namespace Seed
