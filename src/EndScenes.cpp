#include "EndScenes.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "LocalizationManager.h"
#include "GameplayScene.h"

LevelUpScene::LevelUpScene(Game* g) : game(g) {}
void LevelUpScene::Update(float dt) {
    if (InputManager::Instance().IsActionPressed(ACTION_CONFIRM)) {
        game->ChangeScene(std::make_unique<GameplayScene>(game));
    }
}
void LevelUpScene::Draw() {
    Texture2D texOver = ResourceManager::Instance().GetTexture("resources/ui/GameOverScreen.png");
    DrawTexture(texOver, SCREEN_X/2-texOver.width/2, SCREEN_Y/2-texOver.height/2, WHITE);
    std::string msg = (game->level == 10 ? LocalizationManager::Instance().Get("MSG_BOSS_DEFEATED") : TextFormat(LocalizationManager::Instance().Get("MSG_LEVEL_COMPLETE").c_str(), game->level));
    DrawText(msg.c_str(), SCREEN_X/2-120, SCREEN_Y/2-20, 30, YELLOW);
    DrawText(LocalizationManager::Instance().Get("MSG_PRESS_ENTER_CONTINUE").c_str(), SCREEN_X/2-130, SCREEN_Y/2+100, 20, RAYWHITE);
}

LostScene::LostScene(Game* g) : game(g) {}
void LostScene::Update(float dt) {
    if (InputManager::Instance().IsActionPressed(ACTION_CONFIRM)) {
        game->ChangeScene(std::make_unique<GameplayScene>(game));
    }
}
void LostScene::Draw() {
    Texture2D texOver = ResourceManager::Instance().GetTexture("resources/ui/GameOverScreen.png");
    DrawTexture(texOver, SCREEN_X/2-texOver.width/2, SCREEN_Y/2-texOver.height/2, WHITE);
    DrawText(LocalizationManager::Instance().Get("MSG_GAME_OVER").c_str(), SCREEN_X/2-100, SCREEN_Y/2-20, 30, RED);
    DrawText(LocalizationManager::Instance().Get("MSG_TRY_AGAIN").c_str(), SCREEN_X/2-180, SCREEN_Y/2+100, 20, RED);
}
