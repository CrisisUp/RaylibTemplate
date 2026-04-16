#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>
#include <map>

namespace Ralph {

class ConfigManager {
public:
    static ConfigManager& Instance() {
        static ConfigManager instance;
        return instance;
    }

    void Load(const std::string& fileName);
    void Save(const std::string& fileName);

    float GetFloat(const std::string& key, float defaultValue);
    int GetInt(const std::string& key, int defaultValue);
    std::string GetString(const std::string& key, const std::string& defaultValue);

    void SetFloat(const std::string& key, float value) { settings[key] = std::to_string(value); }
    void SetInt(const std::string& key, int value) { settings[key] = std::to_string(value); }
    void SetString(const std::string& key, const std::string& value) { settings[key] = value; }

private:
    ConfigManager() {}
    std::map<std::string, std::string> settings;
    std::string Trim(const std::string& s);
};

} // namespace Ralph

#endif

