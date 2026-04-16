#include "Game.h"
#include "Profiling.h"

int main() {
    Game game;
    PlayMusicStream(game.music);
    
    while (!WindowShouldClose()) {
        game.Update();
        game.Draw();
        PROFILE_FRAME_MARK;
    }
    
    return 0;
}
