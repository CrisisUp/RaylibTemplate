#include "Utils.h"
#include <filesystem>
#include <cstdlib>

#ifdef _WIN32
#include <shlobj.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

namespace Ralph {

namespace fs = std::filesystem;

std::string Utils::GetUserDataDirectory() {
    std::string path;
#ifdef _WIN32
    char appData[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appData))) {
        path = std::string(appData) + "\\RalphArcade";
    }
#elif defined(__APPLE__)
    const char* home = getenv("HOME");
    if (home) {
        path = std::string(home) + "/Library/Application Support/RalphArcade";
    }
#else
    const char* home = getenv("HOME");
    if (home) {
        path = std::string(home) + "/.local/share/RalphArcade";
    }
#endif

    if (!path.empty() && !fs::exists(path)) {
        fs::create_directories(path);
    }
    return path;
}

std::string Utils::GetSavePath(const std::string& fileName) {
    std::string dir = GetUserDataDirectory();
    if (dir.empty()) return fileName;
    return dir + "/" + fileName;
}

} // namespace Ralph
