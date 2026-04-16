#include "Game.h"
#include "MenuScene.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "ConfigManager.h"
#include "LocalizationManager.h"
#include <fstream>

Game::Game() {
    InitWindow(SCREEN_X, SCREEN_Y, WINDOW_TITLE);
    InitAudioDevice();
    InputManager::Instance().Init();
    ConfigManager::Instance().Load("config.txt");
    
    std::string lang = ConfigManager::Instance().GetString("language", "en");
    LocalizationManager::Instance().LoadLanguage(lang);
    
    // Initialize background music
    music = ResourceManager::Instance().GetMusic("resources/sound/arcade.wav");
    
    hiScore = LoadHighScore();
    SetTargetFPS(60);
    
    fadeAlpha = 0;
    isFading = false;
    fadeOut = false;
    
    currentScene = std::make_unique<MenuScene>(this);
}

Game::~Game() {
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
    float dt = GetFrameTime();

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
    BeginDrawing();
    ClearBackground(BLACK);
    
    if (currentScene) currentScene->Draw();
    
    if (fadeAlpha > 0) DrawRectangle(0, 0, SCREEN_X, SCREEN_Y, Fade(BLACK, fadeAlpha));
    
    EndDrawing();
}

int Game::LoadHighScore() {
    int h = 0;
    std::ifstream f("highscore.txt");
    if (f.is_open()) { f >> h; f.close(); }
    return h;
}

void Game::SaveHighScore(int s) {
    std::ofstream f("highscore.txt");
    if (f.is_open()) { f << s; f.close(); }
}
