#include "OptionsScene.h"
#include "InputManager.h"
#include "ConfigManager.h"
#include "LocalizationManager.h"
#include "MenuScene.h"
#include "ResourceManager.h"
#include "Utils.h"
#include <raymath.h>
#include <vector>

namespace Ralph {

OptionsScene::OptionsScene(Game* g) : game(g) {
    musicVol = ConfigManager::Instance().GetInt("musicVolume", 100);
    sfxVol = ConfigManager::Instance().GetInt("sfxVolume", 100);
    fullscreen = IsWindowFullscreen(); // Pega estado real da janela
    lang = ConfigManager::Instance().GetString("language", "pt");
}

void OptionsScene::Update(float dt) {
    if (rebindingMode) {
        if (selectedRebind >= 0) {
            int key = InputManager::Instance().GetAnyKeyPressed();
            if (key > 0) {
                GameAction actions[] = {MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, ACTION_DASH};
                InputManager::Instance().Rebind(actions[selectedRebind], key);
                selectedRebind = -1; // Done rebinding this one
            }
        } else {
            if (InputManager::Instance().IsActionPressed(MOVE_UP)) selectedRebind--;
            if (InputManager::Instance().IsActionPressed(MOVE_DOWN)) selectedRebind++;
            if (selectedRebind < -1) selectedRebind = 4;
            if (selectedRebind > 4) selectedRebind = -1;

            if (selectedRebind >= 0 && InputManager::Instance().IsActionPressed(ACTION_CONFIRM)) {
                // Keep selectedRebind as is, next frame it will detect key
            }

            if (InputManager::Instance().IsActionPressed(MOVE_LEFT)) {
                rebindingMode = false;
            }
        }
        return;
    }

    if (InputManager::Instance().IsActionPressed(MOVE_UP)) selectedOption--;
    if (InputManager::Instance().IsActionPressed(MOVE_DOWN)) selectedOption++;
    if (selectedOption < 0) selectedOption = 4;
    if (selectedOption > 4) selectedOption = 0;

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
        } else if (selectedOption == 2) { // Fullscreen
            fullscreen = !fullscreen;
            ToggleFullscreen();
        } else if (selectedOption == 3) { // Language
            lang = (lang == "pt" ? "en" : "pt");
            LocalizationManager::Instance().LoadLanguage(lang);
        }
    }

    if (selectedOption == 4 && InputManager::Instance().IsActionPressed(ACTION_CONFIRM)) {
        rebindingMode = true;
        selectedRebind = 0;
    } else if (InputManager::Instance().IsActionPressed(ACTION_CONFIRM)) {
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
    int startY = 150;
    int spacing = 50;

    if (rebindingMode) {
        DrawText(LocalizationManager::Instance().Get("OPT_CONTROLS_TITLE").c_str(), centerX - 150, 80, 40, RAYWHITE);
        
        GameAction actions[] = {MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, ACTION_DASH};
        const char* labels[] = {"OPT_MOVE_UP", "OPT_MOVE_DOWN", "OPT_MOVE_LEFT", "OPT_MOVE_RIGHT", "OPT_ACTION_DASH"};
        
        for (int i = 0; i < 5; i++) {
            Color c = (selectedRebind == i) ? YELLOW : RAYWHITE;
            std::string keyName = InputManager::Instance().GetActionKeyName(actions[i]);
            DrawText(TextFormat(LocalizationManager::Instance().Get(labels[i]).c_str(), keyName.c_str()), centerX - 200, startY + i * spacing, 30, c);
        }

        if (selectedRebind >= 0) {
            DrawText(LocalizationManager::Instance().Get("OPT_REBIND_HINT").c_str(), centerX - 150, 500, 20, YELLOW);
        } else {
            DrawText(LocalizationManager::Instance().Get("OPT_REBIND_BACK").c_str(), centerX - 150, 550, 20, GRAY);
        }
        return;
    }

    DrawText(LocalizationManager::Instance().Get("OPT_TITLE").c_str(), centerX - 100, 80, 40, RAYWHITE);

    // Renderiza as opções
    Color colors[5] = {RAYWHITE, RAYWHITE, RAYWHITE, RAYWHITE, RAYWHITE};
    colors[selectedOption] = YELLOW;

    DrawText(TextFormat(LocalizationManager::Instance().Get("OPT_MUSIC_VOL").c_str(), musicVol), centerX - 200, startY, 30, colors[0]);
    DrawText(TextFormat(LocalizationManager::Instance().Get("OPT_SFX_VOL").c_str(), sfxVol), centerX - 200, startY + spacing, 30, colors[1]);
    DrawText(TextFormat(LocalizationManager::Instance().Get("OPT_FULLSCREEN").c_str(), 
             fullscreen ? LocalizationManager::Instance().Get("ON").c_str() : LocalizationManager::Instance().Get("OFF").c_str()), 
             centerX - 200, startY + spacing * 2, 30, colors[2]);
    DrawText(TextFormat(LocalizationManager::Instance().Get("OPT_LANG").c_str(), lang.c_str()), centerX - 200, startY + spacing * 3, 30, colors[3]);
    DrawText(LocalizationManager::Instance().Get("OPT_CONTROLS").c_str(), centerX - 200, startY + spacing * 4, 30, colors[4]);

    DrawText(LocalizationManager::Instance().Get("OPT_BACK").c_str(), centerX - 200, 550, 20, GRAY);
}

} // namespace Ralph
