#ifndef GAMEPLAY_SCENE_H
#define GAMEPLAY_SCENE_H

#include "Scene.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "ParticleManager.h"
#include "FloatingTextManager.h"
#include <vector>

class GameplayScene : public Scene {
public:
    GameplayScene(Game* game);
    void Update(float dt) override;
    void Draw() override;

private:
    Game* game;
    std::unique_ptr<Player> player;
    std::vector<Enemy> enemies;
    std::vector<Vector2> obstacles, breakables;
    Boss boss;
    ParticleManager pm;
    FloatingTextManager ftm;
    
    struct PowerUp { Vector2 pos; PowerUpType type; bool active; } pUp;
    float pUpTimer;
    float freezeTimer;
    float flashAlpha;
    float shakeTimer, shakeIntensity;
    float comboTimer;

    void ResetLevel();
    void TriggerImpact(float duration, float intensity, float flash);
};

#endif
