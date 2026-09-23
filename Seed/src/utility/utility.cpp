#include "utility.h"
#include <fstream>
#include <pwd.h>
#include <sys/stat.h>

namespace Seed {

void Asset_Name(const std::string &f, std::string &filename) {
    auto slashPos = f.find_last_of("/");
    auto dotPos = f.find_last_of(".");
    std::string ss = (dotPos != std::string::npos) ? f.substr(0, dotPos) : f;

    filename.length() != 0 ? filename = (slashPos == std::string::npos) ? ss : ss.substr(slashPos + 1) : filename;
};

void Read_File(const std::string &f, std::string &s) {
    std::ifstream fs(f, std::ios::binary);
    // SEED_CORE_ASSERT(fs, "No valid filepath");

    if (fs) {
        fs.seekg(0, std::ios::end);
        s.resize(fs.tellg());
        fs.seekg(0, std::ios::beg);
        fs.read(&s[0], s.size());
        fs.close();
    }
};

std::string GetConfigDir() {
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

bool CreateDirs(const std::string &path) {
    size_t pos = 0;
    while ((pos = path.find_first_of('/', pos + 1)) != std::string::npos) {
        mkdir(path.substr(0, pos).c_str(), 0755);
    }
    return mkdir(path.c_str(), 0755) == 0 || errno == EEXIST;
}

} // namespace Seed
