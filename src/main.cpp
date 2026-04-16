#include "Game.h"

int main() {
    Game game;
    PlayMusicStream(game.music);
    
    while (!WindowShouldClose()) {
        game.Update();
        game.Draw();
    }
    
    return 0;
}
