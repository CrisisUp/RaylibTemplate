#ifndef FLOATING_TEXT_MANAGER_H
#define FLOATING_TEXT_MANAGER_H

#include "Common.h"
#include <vector>
#include <string>

struct FloatingText {
    Vector2 pos;
    std::string text;
    Color color;
    float life, maxLife;
    bool active;
};

class FloatingTextManager {
public:
    std::vector<FloatingText> texts;
    void Spawn(Vector2 pos, std::string text, Color color);
    void Update(float dt);
    void Draw(Vector2 offset);
    void Clear();
};

#endif
