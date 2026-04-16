#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include "ParticleManager.h"
#include <vector>

class Player {
public:
    Vector2 pos, startPos;
    Texture2D texture;
    int lives;
    float invulnTimer, walkTimer, rotation, scaleY;
    bool facingRight, godMode;
    PowerUpType activePower;
    float powerTimer, dashTimer, dashCooldown;

    Player(Texture2D tex);
    void Reset();
    void Update(float dt, float baseSpeed, const std::vector<Vector2>& obstacles, float obsW, float obsH, ParticleManager& pm);
    void Draw(Vector2 shake);
    Rectangle GetRect();
};

#endif
