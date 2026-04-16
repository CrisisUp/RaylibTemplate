#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "Scene.h"
#include <memory>

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

private:
    float fadeAlpha;
    bool isFading, fadeOut;
    std::unique_ptr<Scene> nextScenePending;
};

#endif
