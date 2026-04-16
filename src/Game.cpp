#include "Game.h"
#include "MenuScene.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "ConfigManager.h"
#include "LocalizationManager.h"
#include "Profiling.h"
#include "Logger.h"
#include "Utils.h"
#include <fstream>
#include <raymath.h>

Game::Game() {
    // Permite que a janela seja redimensionada e use anti-aliasing
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_X, SCREEN_Y, WINDOW_TITLE);
    InitAudioDevice();
    
    // Inicializa a textura de renderização (tela virtual interna)
    target = LoadRenderTexture(SCREEN_X, SCREEN_Y);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
    
    InputManager::Instance().Init();
    
    // Tenta carregar o pacote de assets primeiro
    ResourceManager::Instance().LoadPak("resources.pak");
    
    // Tenta carregar config do perfil do usuário, senao da pasta do jogo
    std::string configPath = Utils::GetSavePath("config.txt");
    if (std::filesystem::exists(configPath)) {
        ConfigManager::Instance().Load(configPath);
    } else {
        ConfigManager::Instance().Load("config.txt");
    }
    
    // Aplica configurações de áudio
    int musicVol = ConfigManager::Instance().GetInt("musicVolume", 100);
    
    std::string lang = ConfigManager::Instance().GetString("language", "pt");
    LocalizationManager::Instance().LoadLanguage(lang);
    
    // Initialize background music
    music = ResourceManager::Instance().GetMusic("resources/sound/arcade.wav");
    SetMusicVolume(music, musicVol / 100.0f);
    
    hiScore = LoadHighScore();
    score = 0;
    level = 1;
    dispScore = 0;
    SetTargetFPS(60);
    
    fadeAlpha = 0;
    isFading = false;
    fadeOut = false;
    
    currentScene = std::make_unique<MenuScene>(this);
}

Game::~Game() {
    UnloadRenderTexture(target);
    ResourceManager::Instance().UnloadAll();
}

void Game::ChangeScene(std::unique_ptr<Scene> newScene) {
    if (!isFading) {
        nextScenePending = std::move(newScene);
        isFading = true;
        fadeOut = true;
        fadeAlpha = 0;
    }
}

void Game::Update() {
    PROFILE_ZONE_SCOPED;
    float dt = GetFrameTime();

    UpdateMusicStream(music);

    // Calcula a escala da tela para o Letterboxing
    float scale = fminf((float)GetScreenWidth()/SCREEN_X, (float)GetScreenHeight()/SCREEN_Y);
    
    // Define o retângulo de destino centralizado na janela real
    screenRect.width = SCREEN_X * scale;
    screenRect.height = SCREEN_Y * scale;
    screenRect.x = (GetScreenWidth() - screenRect.width) * 0.5f;
    screenRect.y = (GetScreenHeight() - screenRect.height) * 0.5f;

    if (isFading) {
        fadeAlpha += dt * (fadeOut ? 3.0f : -3.0f);
        if (fadeOut && fadeAlpha >= 1.0f) {
            fadeAlpha = 1.0f;
            fadeOut = false;
            currentScene = std::move(nextScenePending);
        } else if (!fadeOut && fadeAlpha <= 0) {
            fadeAlpha = 0;
            isFading = false;
        }
        return;
    }

    if (currentScene) currentScene->Update(dt);
}

void Game::Draw() {
    PROFILE_ZONE_SCOPED;
    
    // 1. Desenha tudo na textura virtual (tela de 1280x700)
    BeginTextureMode(target);
        ClearBackground(BLACK);
        if (currentScene) currentScene->Draw();
        if (fadeAlpha > 0) DrawRectangle(0, 0, SCREEN_X, SCREEN_Y, Fade(BLACK, fadeAlpha));
    EndTextureMode();

    // 2. Desenha a textura virtual na janela real, escalando e centralizando
    BeginDrawing();
        ClearBackground(DARKGRAY); // Cor de fundo das barras (letterbox)
        
        // Invertemos o Y (negativo na altura da origem) porque texturas no OpenGL são invertidas
        Rectangle sourceRec = { 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height };
        
        // Desenha com a escala calculada no Update
        DrawTexturePro(target.texture, sourceRec, screenRect, { 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}

int Game::LoadHighScore() {
    int h = 0;
    std::ifstream f(Utils::GetSavePath("highscore.txt"));
    if (f.is_open()) { f >> h; f.close(); }
    return h;
}

void Game::SaveHighScore(int s) {
    std::ofstream f(Utils::GetSavePath("highscore.txt"));
    if (f.is_open()) { f << s; f.close(); }
}
