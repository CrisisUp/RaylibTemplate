#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "Scene.h"
#include <memory>

namespace Ralph {

class Game {
public:
    int score, hiScore, level, comboCount;
    float dispScore;
    
    Music music; // Declare music here
    std::unique_ptr<Scene> currentScene;
    
    Game();
    ~Game();
    
    void Update();
    void Draw();
    
    void ChangeScene(std::unique_ptr<Scene> newScene);
    
    // Global Save/Load
    int LoadHighScore();
    void SaveHighScore(int s);

    // Shader Effects
    void TriggerDamageEffect() { aberrationAmount = 1.0f; }

private:
    float fadeAlpha;
    bool isFading, fadeOut;
    std::unique_ptr<Scene> nextScenePending;

    // Resolution Independence
    RenderTexture2D target;
    Rectangle screenRect;

    // Shader Pipeline
    Shader retroShader;
    Shader bloomShader;
    float aberrationAmount = 0.0f;
};

} // namespace Ralph

#endif
