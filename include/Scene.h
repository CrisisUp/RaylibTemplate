#ifndef SCENE_H
#define SCENE_H

class Scene {
public:
    virtual ~Scene() {}
    virtual void Update(float dt) = 0;
    virtual void Draw() = 0;
};

#endif
