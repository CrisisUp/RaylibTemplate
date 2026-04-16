#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include "Common.h"
#include <vector>

namespace Ralph {

struct Particle {
    Vector2 pos, speed;
    Color color;
    float life, size;
    bool active;
};

class ParticleManager {
public:
    std::vector<Particle> particles;
    void SpawnBurst(Vector2 position, Color color, int count);
    void Update(float dt);
    void Draw(Vector2 offset);
    void Clear();
};

} // namespace Ralph

#endif
