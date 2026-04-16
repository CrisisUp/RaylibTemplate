#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Common.h"
#include <map>

enum GameAction {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    ACTION_DASH,
    ACTION_START,
    ACTION_CONFIRM,
    ACTION_PAUSE
};

class InputManager {
public:
    static InputManager& Instance() {
        static InputManager instance;
        return instance;
    }

    // Initialize with default keys
    void Init();

    // Check if an action is currently held down
    bool IsActionDown(GameAction action);

    // Check if an action was just pressed this frame
    bool IsActionPressed(GameAction action);

    // Rebind an action to a new key
    void Rebind(GameAction action, int newKey);

private:
    InputManager() {}
    std::map<GameAction, int> bindings;
};

#endif
