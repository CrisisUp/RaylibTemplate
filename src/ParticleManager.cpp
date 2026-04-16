#include "ParticleManager.h"
#include <raymath.h>

namespace Ralph {

void ParticleManager::SpawnBurst(Vector2 position, Color color, int count) {
    for (int i = 0; i < count; i++) {
        particles.push_back({
            position,
            {(float)GetRandomValue(-300, 300), (float)GetRandomValue(-300, 300)},
            color,
            (float)GetRandomValue(5, 15) / 10.0f,
            (float)GetRandomValue(3, 10),
            true
        });
    }
}

void ParticleManager::Update(float dt) {
    for (auto it = particles.begin(); it != particles.end();) {
        if (it->active) {
            it->pos = Vector2Add(it->pos, Vector2Scale(it->speed, dt));
            it->life -= dt;
            if (it->life <= 0) it->active = false;
            ++it;
        } else {
            it = particles.erase(it);
        }
    }
}

void ParticleManager::Draw(Vector2 offset) {
    for (const auto& p : particles) {
        DrawRectangleV(Vector2Add(p.pos, offset), {p.size, p.size}, Fade(p.color, p.life));
    }
}

void ParticleManager::Clear() {
    particles.clear();
}

} // namespace Ralph
