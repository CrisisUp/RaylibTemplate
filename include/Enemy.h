#ifndef ENEMY_H
#define ENEMY_H

#include "Common.h"

class Enemy {
public:
    Vector2 pos, speed;
    EnemyType type;
    float width, height;
    bool active;

    Enemy(Vector2 p, Vector2 s, float w, float h, EnemyType t);
    void Update(float dt, Vector2 playerPos, float mult, bool slow);
    void Draw(Texture2D tex, Vector2 shake);
    Rectangle GetRect();
};

#endif
