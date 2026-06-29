#pragma once

#include <cstdio>
#include <future>
#include <source_location>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

namespace Seed {

enum class LogLevel { Trace, Debug, Info, Warn, Error, Fatal };

class Logger {
public:
    static Logger &Get();
    void SetLevel(LogLevel level);
    void SetQuiet(bool quiet);
    void Shutdown();
    void FlSync();

    void Log(LogLevel level, std::source_location loc, const char *fmt, ...)
        __attribute__((format(printf, 4, 5)));

private:
    Logger();
    ~Logger();
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    struct LogMessage {
        std::string text;
    };

    void ThreadFunc();
    std::string FormatPrefix(LogLevel level, std::source_location loc);

    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::queue<LogMessage> m_queue;
    std::queue<std::promise<void>> m_flReq;
    std::thread m_thread;
    FILE *m_file = nullptr;
    LogLevel m_level = LogLevel::Trace;
    bool m_quiet = false;
    bool m_running = true;

    static constexpr size_t kBatchSize = 64 * 1024;
    std::string m_batchBuffer;
    std::chrono::steady_clock::time_point m_lastFlush;
};

} // namespace Seed

#define Seed_Trace(fmt, ...)                                                            \
    ::Seed::Logger::Get().Log(::Seed::LogLevel::Trace, std::source_location::current(), \
                              fmt __VA_OPT__(, ) __VA_ARGS__)
#define Seed_Debug(fmt, ...)                                                            \
    ::Seed::Logger::Get().Log(::Seed::LogLevel::Debug, std::source_location::current(), \
                              fmt __VA_OPT__(, ) __VA_ARGS__)
#define Seed_Info(fmt, ...)                                                            \
    ::Seed::Logger::Get().Log(::Seed::LogLevel::Info, std::source_location::current(), \
                              fmt __VA_OPT__(, ) __VA_ARGS__)
#define Seed_Warn(fmt, ...)                                                            \
    ::Seed::Logger::Get().Log(::Seed::LogLevel::Warn, std::source_location::current(), \
                              fmt __VA_OPT__(, ) __VA_ARGS__)
#define Seed_Error(fmt, ...)                                                            \
    ::Seed::Logger::Get().Log(::Seed::LogLevel::Error, std::source_location::current(), \
                              fmt __VA_OPT__(, ) __VA_ARGS__)
#define Seed_Fatal(fmt, ...)                                                                \
    do {                                                                                    \
        ::Seed::Logger::Get().Log(::Seed::LogLevel::Fatal, std::source_location::current(), \
                                  fmt __VA_OPT__(, ) __VA_ARGS__);                          \
        ::Seed::Logger::Get().FlSync();                                                     \
        abort();                                                                            \
    } while (0)
