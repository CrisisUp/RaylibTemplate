#ifndef SCENE_H
#define SCENE_H

namespace Ralph {

class Scene {
public:
    virtual ~Scene() {}
    virtual void Update(float dt) = 0;
    virtual void Draw() = 0;
};

} // namespace Ralph

#endif
