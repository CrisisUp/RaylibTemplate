#include "ResourceManager.h"
#include <cstdio>
#include <cstring>
#include <iostream>

void ResourceManager::LoadPak(const std::string& pakPath) {
    FILE* f = fopen(pakPath.c_str(), "rb");
    if (!f) return;

    char magic[5] = {0};
    fread(magic, 1, 4, f);
    if (strcmp(magic, "RPAK") != 0) {
        fclose(f);
        return;
    }

    unsigned int version, numFiles;
    fread(&version, 4, 1, f);
    fread(&numFiles, 4, 1, f);

    for (unsigned int i = 0; i < numFiles; i++) {
        char name[64];
        unsigned int offset, size;
        fread(name, 1, 64, f);
        fread(&offset, 4, 1, f);
        fread(&size, 4, 1, f);
        pakIndex[std::string(name)] = {offset, size};
    }

    fclose(f);
    currentPakPath = pakPath;
    usingPak = true;
    std::cout << "ResourceManager: Loaded " << numFiles << " assets from " << pakPath << std::endl;
}

unsigned char* ResourceManager::LoadFromPak(const std::string& fileName, int* size) {
    if (!usingPak || pakIndex.find(fileName) == pakIndex.end()) return nullptr;

    ResourceEntry entry = pakIndex[fileName];
    FILE* f = fopen(currentPakPath.c_str(), "rb");
    if (!f) return nullptr;

    fseek(f, entry.offset, SEEK_SET);
    unsigned char* data = (unsigned char*)malloc(entry.size);
    fread(data, 1, entry.size, f);
    fclose(f);

    *size = (int)entry.size;
    return data;
}

Texture2D ResourceManager::GetTexture(const std::string& fileName) {
    if (textures.find(fileName) == textures.end()) {
        int size;
        unsigned char* data = LoadFromPak(fileName, &size);
        if (data) {
            const char* ext = strrchr(fileName.c_str(), '.');
            Image img = LoadImageFromMemory(ext, data, size);
            textures[fileName] = LoadTextureFromImage(img);
            UnloadImage(img);
            free(data);
        } else {
            textures[fileName] = LoadTexture(fileName.c_str());
        }
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
        int size;
        unsigned char* data = LoadFromPak(fileName, &size);
        if (data) {
            const char* ext = strrchr(fileName.c_str(), '.');
            Wave wave = LoadWaveFromMemory(ext, data, size);
            sounds[fileName] = LoadSoundFromWave(wave);
            UnloadWave(wave);
            free(data);
        } else {
            sounds[fileName] = LoadSound(fileName.c_str());
        }
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
        int size;
        unsigned char* data = LoadFromPak(fileName, &size);
        if (data) {
            // Nota: Para Music Stream, raylib precisa que o arquivo esteja no disco para streaming.
            // Para simplificar no prototipo, carregamos do disco se falhar do pak.
            // Em versoes profissionais, usariamos LoadMusicStreamFromMemory se suportado (requer raylib 5.0+).
            const char* ext = strrchr(fileName.c_str(), '.');
            musicStreams[fileName] = LoadMusicStreamFromMemory(ext, data, size);
            free(data);
        } else {
            musicStreams[fileName] = LoadMusicStream(fileName.c_str());
        }
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
