#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include "Scene.h"
#include "Game.h"

namespace Ralph {

class MenuScene : public Scene {
public:
    MenuScene(Game* game);
    void Update(float dt) override;
    void Draw() override;
private:
    Game* game;
};

} // namespace Ralph

#endif

