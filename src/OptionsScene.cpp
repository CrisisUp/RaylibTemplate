#include "OptionsScene.h"
#include "InputManager.h"
#include "ConfigManager.h"
#include "LocalizationManager.h"
#include "MenuScene.h"
#include "ResourceManager.h"
#include "Utils.h"
#include <raymath.h>

namespace Ralph {

OptionsScene::OptionsScene(Game* g) : game(g) {
    musicVol = ConfigManager::Instance().GetInt("musicVolume", 100);
    sfxVol = ConfigManager::Instance().GetInt("sfxVolume", 100);
    fullscreen = IsWindowFullscreen(); // Pega estado real da janela
    lang = ConfigManager::Instance().GetString("language", "pt");
}

void OptionsScene::Update(float dt) {
    if (InputManager::Instance().IsActionPressed(MOVE_UP)) selectedOption--;
    if (InputManager::Instance().IsActionPressed(MOVE_DOWN)) selectedOption++;
    if (selectedOption < 0) selectedOption = 3;
    if (selectedOption > 3) selectedOption = 0;

    // Lógica de alteração
    int change = 0;
    if (InputManager::Instance().IsActionPressed(MOVE_LEFT)) change = -1;
    if (InputManager::Instance().IsActionPressed(MOVE_RIGHT)) change = 1;

    if (change != 0) {
        if (selectedOption == 0) { // Music
            musicVol = Clamp(musicVol + change * 10, 0, 100);
            SetMusicVolume(game->music, musicVol / 100.0f);
        } else if (selectedOption == 1) { // SFX
            sfxVol = Clamp(sfxVol + change * 10, 0, 100);
            // Aqui poderíamos aplicar volume global de sons se houvesse suporte
        } else if (selectedOption == 2) { // Fullscreen
            fullscreen = !fullscreen;
            ToggleFullscreen();
        } else if (selectedOption == 3) { // Language
            lang = (lang == "pt" ? "en" : "pt");
            LocalizationManager::Instance().LoadLanguage(lang);
        }
    }

    if (InputManager::Instance().IsActionPressed(ACTION_CONFIRM)) {
        SaveAndExit();
    }
}

void OptionsScene::SaveAndExit() {
    ConfigManager::Instance().SetInt("musicVolume", musicVol);
    ConfigManager::Instance().SetInt("sfxVolume", sfxVol);
    ConfigManager::Instance().SetString("language", lang);
    ConfigManager::Instance().Save(Utils::GetSavePath("config.txt"));
    game->ChangeScene(std::make_unique<MenuScene>(game));
}

void OptionsScene::Draw() {
    ClearBackground(BLACK);
    
    int centerX = SCREEN_X / 2;
    int startY = 200;
    int spacing = 60;

    DrawText(LocalizationManager::Instance().Get("OPT_TITLE").c_str(), centerX - 100, 100, 40, RAYWHITE);

    // Renderiza as opções
    Color colors[4] = {RAYWHITE, RAYWHITE, RAYWHITE, RAYWHITE};
    colors[selectedOption] = YELLOW;

    DrawText(TextFormat(LocalizationManager::Instance().Get("OPT_MUSIC_VOL").c_str(), musicVol), centerX - 200, startY, 30, colors[0]);
    DrawText(TextFormat(LocalizationManager::Instance().Get("OPT_SFX_VOL").c_str(), sfxVol), centerX - 200, startY + spacing, 30, colors[1]);
    DrawText(TextFormat(LocalizationManager::Instance().Get("OPT_FULLSCREEN").c_str(), 
             fullscreen ? LocalizationManager::Instance().Get("ON").c_str() : LocalizationManager::Instance().Get("OFF").c_str()), 
             centerX - 200, startY + spacing * 2, 30, colors[2]);
    DrawText(TextFormat(LocalizationManager::Instance().Get("OPT_LANG").c_str(), lang.c_str()), centerX - 200, startY + spacing * 3, 30, colors[3]);

    DrawText(LocalizationManager::Instance().Get("OPT_BACK").c_str(), centerX - 200, 550, 20, GRAY);
}

} // namespace Ralph
