#include "FloatingTextManager.h"

void FloatingTextManager::Spawn(Vector2 pos, std::string text, Color color) {
    texts.push_back({pos, text, color, 1.0f, 1.0f, true});
}

void FloatingTextManager::Update(float dt) {
    for (auto it = texts.begin(); it != texts.end();) {
        if (it->active) {
            it->pos.y -= 40.0f * dt;
            it->life -= dt;
            if (it->life <= 0) it->active = false;
            ++it;
        } else {
            it = texts.erase(it);
        }
    }
}

void FloatingTextManager::Draw(Vector2 offset) {
    for (const auto& t : texts) {
        DrawText(t.text.c_str(), (int)(t.pos.x + offset.x), (int)(t.pos.y + offset.y), 20, Fade(t.color, t.life / t.maxLife));
    }
}

void FloatingTextManager::Clear() {
    texts.clear();
}
