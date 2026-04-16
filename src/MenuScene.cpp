#include "MenuScene.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "LocalizationManager.h"
#include "GameplayScene.h"

MenuScene::MenuScene(Game* g) : game(g) {
}

void MenuScene::Update(float dt) {
    if (InputManager::Instance().IsActionPressed(ACTION_START)) {
        game->ChangeScene(std::make_unique<GameplayScene>(game));
    }
}

void MenuScene::Draw() {
    Texture2D texMenu = ResourceManager::Instance().GetTexture("resources/ui/MenuScreen.png");
    DrawTexture(texMenu, SCREEN_X/2 - texMenu.width/2, SCREEN_Y/2 - texMenu.height/2, WHITE);
    
    DrawText(LocalizationManager::Instance().Get("MENU_START").c_str(), SCREEN_X/2 - 130, SCREEN_Y/2 + 100, 20, RAYWHITE);
    DrawText(TextFormat("%s: %d", LocalizationManager::Instance().Get("MENU_BEST_SCORE").c_str(), game->hiScore), SCREEN_X/2 - 80, SCREEN_Y/2 + 150, 20, YELLOW);
}
