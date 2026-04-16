#ifndef OPTIONS_SCENE_H
#define OPTIONS_SCENE_H

#include "Scene.h"
#include "Game.h"
#include <string>

namespace Ralph {

class OptionsScene : public Scene {
public:
    OptionsScene(Game* g);
    void Update(float dt) override;
    void Draw() override;

private:
    Game* game;
    int selectedOption = 0;
    
    int musicVol;
    int sfxVol;
    bool fullscreen;
    std::string lang;

    void SaveAndExit();
};

} // namespace Ralph

#endif
