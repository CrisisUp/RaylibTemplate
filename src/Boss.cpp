#include "Boss.h"
#include <raymath.h>

Boss::Boss() : active(false) {
    maxHp = 10;
    hp = maxHp;
    width = 150;
    height = 150;
    speed = 120.0f;
    pos = {(float)SCREEN_X/2 - width/2, 150};
}

void Boss::Update(float dt, Vector2 playerPos) {
    if (!active) return;
    Vector2 dir = Vector2Normalize(Vector2Subtract(playerPos, {pos.x + width/2, pos.y + height/2}));
    pos = Vector2Add(pos, Vector2Scale(dir, speed * dt));
}

void Boss::Draw(Texture2D tex, Vector2 shake) {
    if (!active) return;
    DrawTexturePro(tex, {0,0,(float)tex.width, (float)tex.height}, 
                  {pos.x + shake.x, pos.y + shake.y, width, height}, {0,0}, 0, MAROON);
    
    DrawRectangle(pos.x + shake.x, pos.y + shake.y - 20, width, 10, BLACK);
    DrawRectangle(pos.x + shake.x, pos.y + shake.y - 20, (float)hp / (float)maxHp * width, 10, RED);
}

Rectangle Boss::GetRect() {
    return {pos.x, pos.y, width, height};
}
