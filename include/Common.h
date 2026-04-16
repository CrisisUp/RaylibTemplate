#ifndef COMMON_H
#define COMMON_H

#include <raylib.h>

#define SCREEN_X 1280
#define SCREEN_Y 700
#define WINDOW_TITLE "Ralph Arcade - FINAL REVISED"

namespace Ralph {
    enum GameState { MENU, PLAYING, LEVEL_UP, LOST };
    enum EnemyType { BOUNCER, HUNTER };
    enum PowerUpType { SPEED, INVINCIBLE, SLOW, NONE };
}

#endif
