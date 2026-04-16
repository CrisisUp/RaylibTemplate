#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <raylib.h>
#include <map>
#include <string>

class ResourceManager {
public:
    static ResourceManager& Instance() {
        static ResourceManager instance;
        return instance;
    }

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
};

#endif
