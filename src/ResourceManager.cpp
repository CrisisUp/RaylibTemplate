#include "ResourceManager.h"

Texture2D ResourceManager::GetTexture(const std::string& fileName) {
    if (textures.find(fileName) == textures.end()) {
        textures[fileName] = LoadTexture(fileName.c_str());
    }
    return textures[fileName];
}

void ResourceManager::UnloadTextures() {
    for (auto& pair : textures) {
        UnloadTexture(pair.second);
    }
    textures.clear();
}

Sound ResourceManager::GetSound(const std::string& fileName) {
    if (sounds.find(fileName) == sounds.end()) {
        sounds[fileName] = LoadSound(fileName.c_str());
    }
    return sounds[fileName];
}

void ResourceManager::UnloadSounds() {
    for (auto& pair : sounds) {
        UnloadSound(pair.second);
    }
    sounds.clear();
}

Music ResourceManager::GetMusic(const std::string& fileName) {
    if (musicStreams.find(fileName) == musicStreams.end()) {
        musicStreams[fileName] = LoadMusicStream(fileName.c_str());
    }
    return musicStreams[fileName];
}

void ResourceManager::UnloadMusic() {
    for (auto& pair : musicStreams) {
        UnloadMusicStream(pair.second);
    }
    musicStreams.clear();
}

void ResourceManager::UnloadAll() {
    UnloadTextures();
    UnloadSounds();
    UnloadMusic();
}
