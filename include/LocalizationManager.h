#ifndef LOCALIZATION_MANAGER_H
#define LOCALIZATION_MANAGER_H

#include <string>
#include <map>

namespace Ralph {

class LocalizationManager {
public:
    static LocalizationManager& Instance() {
        static LocalizationManager instance;
        return instance;
    }

    void LoadLanguage(const std::string& langCode);
    std::string Get(const std::string& key);

private:
    LocalizationManager() {}
    std::map<std::string, std::string> strings;
    std::string Trim(const std::string& s);
};

} // namespace Ralph

#endif
