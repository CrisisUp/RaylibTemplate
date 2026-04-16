#ifndef END_SCENES_H
#define END_SCENES_H

#include "Scene.h"
#include "Game.h"

namespace Ralph {

class LevelUpScene : public Scene {
public:
    LevelUpScene(Game* g);
    void Update(float dt) override;
    void Draw() override;
private:
    Game* game;
};

class LostScene : public Scene {
public:
    LostScene(Game* g);
    void Update(float dt) override;
    void Draw() override;
private:
    Game* game;
};

} // namespace Ralph

#endif
