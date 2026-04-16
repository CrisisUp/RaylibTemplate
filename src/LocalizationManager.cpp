#include "LocalizationManager.h"
#include <fstream>
#include <iostream>

namespace Ralph {

void LocalizationManager::LoadLanguage(const std::string& langCode) {
    std::string fileName = "resources/lang/" + langCode + ".txt";
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open language file " << fileName << ". Text might be missing.\n";
        return;
    }

    strings.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        size_t sep = line.find('=');
        if (sep != std::string::npos) {
            std::string key = Trim(line.substr(0, sep));
            std::string value = Trim(line.substr(sep + 1));
            strings[key] = value;
        }
    }
    file.close();
}

std::string LocalizationManager::Get(const std::string& key) {
    if (strings.find(key) != strings.end()) {
        return strings[key];
    }
    return "[" + key + "]"; // Return key in brackets if not found
}

std::string LocalizationManager::Trim(const std::string& s) {
    size_t first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, (last - first + 1));
}

} // namespace Ralph
