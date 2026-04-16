#include "InputManager.h"

void InputManager::Init() {
    // Set default keys
    bindings[MOVE_UP]      = KEY_UP;
    bindings[MOVE_DOWN]    = KEY_DOWN;
    bindings[MOVE_LEFT]    = KEY_LEFT;
    bindings[MOVE_RIGHT]   = KEY_RIGHT;
    bindings[ACTION_DASH]  = KEY_SPACE;
    bindings[ACTION_START] = KEY_ENTER;
    bindings[ACTION_CONFIRM] = KEY_ENTER;
    bindings[ACTION_PAUSE] = KEY_P;
    bindings[ACTION_OPTIONS] = KEY_O;
}

bool InputManager::IsActionDown(GameAction action) {
    if (bindings.find(action) != bindings.end()) {
        return IsKeyDown(bindings[action]);
    }
    return false;
}

bool InputManager::IsActionPressed(GameAction action) {
    if (bindings.find(action) != bindings.end()) {
        return IsKeyPressed(bindings[action]);
    }
    return false;
}

void InputManager::Rebind(GameAction action, int newKey) {
    bindings[action] = newKey;
}
