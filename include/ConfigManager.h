#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>
#include <map>

class ConfigManager {
public:
    static ConfigManager& Instance() {
        static ConfigManager instance;
        return instance;
    }

    void Load(const std::string& fileName);
    
    float GetFloat(const std::string& key, float defaultValue);
    int GetInt(const std::string& key, int defaultValue);
    std::string GetString(const std::string& key, const std::string& defaultValue);

private:
    ConfigManager() {}
    std::map<std::string, std::string> settings;
    std::string Trim(const std::string& s);
};

#endif
