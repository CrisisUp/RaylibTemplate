#ifndef BOSS_H
#define BOSS_H

#include "Common.h"

namespace Ralph {

class Boss {
public:
    Vector2 pos;
    int hp, maxHp;
    float width, height, speed;
    bool active;

    Boss();
    void Update(float dt, Vector2 playerPos);
    void Draw(Texture2D tex, Vector2 shake);
    Rectangle GetRect();
};

} // namespace Ralph

#endif
