#include "InputManager.h"
#include "ConfigManager.h"

namespace Ralph {

void InputManager::Init() {
    // Teclas Padrão
    bindings[MOVE_UP]      = ConfigManager::Instance().GetInt("key_up", KEY_UP);
    bindings[MOVE_DOWN]    = ConfigManager::Instance().GetInt("key_down", KEY_DOWN);
    bindings[MOVE_LEFT]    = ConfigManager::Instance().GetInt("key_left", KEY_LEFT);
    bindings[MOVE_RIGHT]   = ConfigManager::Instance().GetInt("key_right", KEY_RIGHT);
    bindings[ACTION_DASH]  = ConfigManager::Instance().GetInt("key_dash", KEY_SPACE);
    bindings[ACTION_START] = KEY_ENTER;
    bindings[ACTION_CONFIRM] = KEY_ENTER;
    bindings[ACTION_PAUSE] = KEY_P;
    bindings[ACTION_OPTIONS] = KEY_O;
}

bool InputManager::IsActionDown(GameAction action) {
    if (bindings.find(action) != bindings.end()) {
        int k = bindings[action];
        return IsKeyDown(k) || (IsGamepadAvailable(0) && (
            (action == MOVE_UP && (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) < -0.5f)) ||
            (action == MOVE_DOWN && (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) > 0.5f)) ||
            (action == MOVE_LEFT && (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) < -0.5f)) ||
            (action == MOVE_RIGHT && (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) > 0.5f)) ||
            (action == ACTION_DASH && IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) ||
            (action == ACTION_START && IsGamepadButtonDown(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) ||
            (action == ACTION_CONFIRM && IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) ||
            (action == ACTION_PAUSE && IsGamepadButtonDown(0, GAMEPAD_BUTTON_MIDDLE_RIGHT))
        ));
    }
    return false;
}

bool InputManager::IsActionPressed(GameAction action) {
    if (bindings.find(action) != bindings.end()) {
        int k = bindings[action];
        return IsKeyPressed(k) || (IsGamepadAvailable(0) && (
            (action == MOVE_UP && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) ||
            (action == MOVE_DOWN && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) ||
            (action == MOVE_LEFT && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) ||
            (action == MOVE_RIGHT && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) ||
            (action == ACTION_DASH && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) ||
            (action == ACTION_START && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) ||
            (action == ACTION_CONFIRM && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) ||
            (action == ACTION_PAUSE && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT))
        ));
    }
    return false;
}

void InputManager::Rebind(GameAction action, int newKey) {
    if (newKey <= 0) return; // Não rebind para tecla nula ou inválida
    
    bindings[action] = newKey;
    
    // Salva automaticamente no config
    if (action == MOVE_UP) ConfigManager::Instance().SetInt("key_up", newKey);
    else if (action == MOVE_DOWN) ConfigManager::Instance().SetInt("key_down", newKey);
    else if (action == MOVE_LEFT) ConfigManager::Instance().SetInt("key_left", newKey);
    else if (action == MOVE_RIGHT) ConfigManager::Instance().SetInt("key_right", newKey);
    else if (action == ACTION_DASH) ConfigManager::Instance().SetInt("key_dash", newKey);
    
    ConfigManager::Instance().Save("config.txt");
}

std::string InputManager::GetActionKeyName(GameAction action) {
    if (bindings.find(action) == bindings.end()) return "NONE";
    
    int key = bindings[action];
    
    // Hardcoded names for Raylib keys
    switch (key) {
        case KEY_SPACE: return "SPACE";
        case KEY_ESCAPE: return "ESCAPE";
        case KEY_ENTER: return "ENTER";
        case KEY_TAB: return "TAB";
        case KEY_BACKSPACE: return "BACKSPACE";
        case KEY_INSERT: return "INSERT";
        case KEY_DELETE: return "DELETE";
        case KEY_RIGHT: return "RIGHT";
        case KEY_LEFT: return "LEFT";
        case KEY_DOWN: return "DOWN";
        case KEY_UP: return "UP";
        case KEY_F1: return "F1";
        case KEY_F2: return "F2";
        case KEY_F3: return "F3";
        case KEY_F4: return "F4";
        case KEY_F5: return "F5";
        case KEY_F6: return "F6";
        case KEY_F7: return "F7";
        case KEY_F8: return "F8";
        case KEY_F9: return "F9";
        case KEY_F10: return "F10";
        case KEY_F11: return "F11";
        case KEY_F12: return "F12";
        case KEY_LEFT_SHIFT: return "L-SHIFT";
        case KEY_LEFT_CONTROL: return "L-CTRL";
        case KEY_LEFT_ALT: return "L-ALT";
        case KEY_RIGHT_SHIFT: return "R-SHIFT";
        case KEY_RIGHT_CONTROL: return "R-CTRL";
        case KEY_RIGHT_ALT: return "R-ALT";
    }

    if (key >= 32 && key <= 126) return std::string(1, (char)key);
    
    return "KEY " + std::to_string(key);
}

int InputManager::GetAnyKeyPressed() {
    return GetKeyPressed();
}

} // namespace Ralph
