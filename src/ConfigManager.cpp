#include "ConfigManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

void ConfigManager::Load(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open config file " << fileName << ". Using defaults.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Ignore comments and empty lines
        if (line.empty() || line[0] == '#') continue;

        size_t sep = line.find('=');
        if (sep != std::string::npos) {
            std::string key = Trim(line.substr(0, sep));
            std::string value = Trim(line.substr(sep + 1));
            settings[key] = value;
        }
    }
    file.close();
}

float ConfigManager::GetFloat(const std::string& key, float defaultValue) {
    if (settings.find(key) != settings.end()) {
        try {
            return std::stof(settings[key]);
        } catch (...) {}
    }
    return defaultValue;
}

int ConfigManager::GetInt(const std::string& key, int defaultValue) {
    if (settings.find(key) != settings.end()) {
        try {
            return std::stoi(settings[key]);
        } catch (...) {}
    }
    return defaultValue;
}

std::string ConfigManager::GetString(const std::string& key, const std::string& defaultValue) {
    if (settings.find(key) != settings.end()) {
        return settings[key];
    }
    return defaultValue;
}

std::string ConfigManager::Trim(const std::string& s) {
    size_t first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, (last - first + 1));
}
