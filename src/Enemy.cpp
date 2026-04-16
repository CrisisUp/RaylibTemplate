#include "Enemy.h"
#include <raymath.h>
#include <cmath>

namespace Ralph {

Enemy::Enemy(Vector2 p, Vector2 s, float w, float h, EnemyType t) 
    : pos(p), speed(s), width(w), height(h), active(true), type(t) {}

void Enemy::Update(float dt, Vector2 playerPos, float mult, bool slow) {
    if (!active) return;
    float sMult = mult * (slow ? 0.4f : 1.0f);
    
    if (type == BOUNCER) {
        // Use speed.x as the total speed for BOUNCER to keep it consistent
        Vector2 velocity = Vector2Scale(Vector2Normalize(speed), speed.x * sMult);
        pos.x += velocity.x * dt;
        pos.y += velocity.y * dt;

        if (pos.x <= 0) { pos.x = 0; speed.x *= -1; }
        if (pos.x + width >= SCREEN_X) { pos.x = SCREEN_X - width; speed.x *= -1; }
        if (pos.y <= 100) { pos.y = 100; speed.y *= -1; }
        if (pos.y + height >= SCREEN_Y) { pos.y = SCREEN_Y - height; speed.y *= -1; }
    } else {
        Vector2 dir = Vector2Normalize(Vector2Subtract(playerPos, pos));
        // Use speed.x as the scalar speed for HUNTER
        pos = Vector2Add(pos, Vector2Scale(dir, speed.x * sMult * dt));
    }
}

void Enemy::Draw(Texture2D tex, Vector2 shake) {
    if (!active) return;
    DrawTexture(tex, (int)(pos.x + shake.x), (int)(pos.y + shake.y), (type == HUNTER ? PURPLE : RED));
}

Rectangle Enemy::GetRect() {
    return {pos.x, pos.y, width, height};
}

} // namespace Ralph
