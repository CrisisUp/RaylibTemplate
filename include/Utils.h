#ifndef UTILS_H
#define UTILS_H

#include <string>

class Utils {
public:
    static std::string GetSavePath(const std::string& fileName);
    static std::string GetUserDataDirectory();
};

#endif
