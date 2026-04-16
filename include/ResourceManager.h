#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <raylib.h>
#include <map>
#include <string>
#include <vector>

struct ResourceEntry {
    unsigned int offset;
    unsigned int size;
};

class ResourceManager {
public:
    static ResourceManager& Instance() {
        static ResourceManager instance;
        return instance;
    }

    // Load the .pak file if it exists
    void LoadPak(const std::string& pakPath);

    // Texture management
    Texture2D GetTexture(const std::string& fileName);
    void UnloadTextures();

    // Sound management
    Sound GetSound(const std::string& fileName);
    void UnloadSounds();

    // Music management
    Music GetMusic(const std::string& fileName);
    void UnloadMusic();

    // Cleanup everything
    void UnloadAll();

private:
    ResourceManager() {}
    ~ResourceManager() { UnloadAll(); }

    std::map<std::string, Texture2D> textures;
    std::map<std::string, Sound> sounds;
    std::map<std::string, Music> musicStreams;

    // Pak file related
    std::string currentPakPath;
    std::map<std::string, ResourceEntry> pakIndex;
    bool usingPak = false;

    unsigned char* LoadFromPak(const std::string& fileName, int* size);
};

#endif
