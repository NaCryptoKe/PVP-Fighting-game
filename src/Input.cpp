#include "src/Input.hpp"
#include <cctype>

static bool g_keys[256] = { false };        // ASCII keys, indexed by lowercase char
static bool g_shiftDown = false;
static bool g_specialKeys[4] = { false, false, false, false };

enum SpecialIndex { SPECIAL_UP = 0, SPECIAL_DOWN = 1, SPECIAL_LEFT = 2, SPECIAL_RIGHT = 3 };

void handleKeyDown(unsigned char key, int x, int y)
{
    g_keys[std::tolower(key)] = true;
    g_shiftDown = (glutGetModifiers() & GLUT_ACTIVE_SHIFT) != 0;
}

void handleKeyUp(unsigned char key, int x, int y)
{
    g_keys[std::tolower(key)] = false;
    g_shiftDown = (glutGetModifiers() & GLUT_ACTIVE_SHIFT) != 0;
}

void handleSpecialKeyDown(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:    g_specialKeys[SPECIAL_UP]    = true; break;
        case GLUT_KEY_DOWN:  g_specialKeys[SPECIAL_DOWN]  = true; break;
        case GLUT_KEY_LEFT:  g_specialKeys[SPECIAL_LEFT]  = true; break;
        case GLUT_KEY_RIGHT: g_specialKeys[SPECIAL_RIGHT] = true; break;
        default: break;
    }
}

void handleSpecialKeyUp(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:    g_specialKeys[SPECIAL_UP]    = false; break;
        case GLUT_KEY_DOWN:  g_specialKeys[SPECIAL_DOWN]  = false; break;
        case GLUT_KEY_LEFT:  g_specialKeys[SPECIAL_LEFT]  = false; break;
        case GLUT_KEY_RIGHT: g_specialKeys[SPECIAL_RIGHT] = false; break;
        default: break;
    }
}

bool isGlutKeyDown(KeyCode code)
{
    switch (code)
    {
        case KeyCode::W:     return g_keys['w'];
        case KeyCode::S:     return g_keys['s'];
        case KeyCode::D:     return g_keys['d'];
        case KeyCode::A:     return g_keys['a'];
        case KeyCode::K:     return g_keys['k'];
        case KeyCode::L:     return g_keys['l'];
        case KeyCode::J:     return g_keys['j'];
        case KeyCode::I:     return g_keys['i'];
        case KeyCode::Space: return g_keys[' '];
        case KeyCode::Shift: return g_shiftDown;

        case KeyCode::Up:    return g_specialKeys[SPECIAL_UP];
        case KeyCode::Down:  return g_specialKeys[SPECIAL_DOWN];
        case KeyCode::Left:  return g_specialKeys[SPECIAL_LEFT];
        case KeyCode::Right: return g_specialKeys[SPECIAL_RIGHT];

        case KeyCode::Num1:  return g_keys['1'];
        case KeyCode::Num2:  return g_keys['2'];
        case KeyCode::Num3:  return g_keys['3'];
        case KeyCode::Num4:  return g_keys['4'];
        case KeyCode::Num5:  return g_keys['5'];
        case KeyCode::Num6:  return g_keys['6'];

        default: return false;
    }
}

void InputManager::setBinding(InputAction action, KeyCode key)
{
    bindings_[action] = key;
    actionStates_[action] = InputState{};
}

void InputManager::applyPlayer1Defaults()
{
    setBinding(InputAction::JUMP, KeyCode::W);
    setBinding(InputAction::CROUCH, KeyCode::S);
    setBinding(InputAction::FORWARD, KeyCode::D);
    setBinding(InputAction::BACKWARD, KeyCode::A);
    setBinding(InputAction::CROSS, KeyCode::K);
    setBinding(InputAction::CIRCLE, KeyCode::L);
    setBinding(InputAction::SQUARE, KeyCode::J);
    setBinding(InputAction::TRIANGLE, KeyCode::I);
    setBinding(InputAction::BLOCK, KeyCode::Space);
    setBinding(InputAction::ENHANCE, KeyCode::Shift);
}

void InputManager::applyPlayer2Defaults()
{
    setBinding(InputAction::JUMP, KeyCode::Up);
    setBinding(InputAction::CROUCH, KeyCode::Down);
    setBinding(InputAction::FORWARD, KeyCode::Right);
    setBinding(InputAction::BACKWARD, KeyCode::Left);
    setBinding(InputAction::CROSS, KeyCode::Num1);
    setBinding(InputAction::CIRCLE, KeyCode::Num2);
    setBinding(InputAction::SQUARE, KeyCode::Num3);
    setBinding(InputAction::TRIANGLE, KeyCode::Num4);
    setBinding(InputAction::BLOCK, KeyCode::Num5);
    setBinding(InputAction::ENHANCE, KeyCode::Num6);
}

void InputManager::update(const std::function<bool(KeyCode)>& isRawKeyDown)
{
    for (auto& [action, key] : bindings_)
    {
        auto& state = actionStates_[action];
        bool isDown = isRawKeyDown(key);

        state.wasPressedThisFrame = isDown && !state.isHeld;
        state.wasReleasedThisFrame = !isDown && state.isHeld;
        state.isHeld = isDown;
    }
}

bool InputManager::isActionHeld(InputAction action) const
{
    auto it = actionStates_.find(action);
    return (it != actionStates_.end()) ? it->second.isHeld : false;
}

bool InputManager::isActionPressed(InputAction action) const
{
    auto it = actionStates_.find(action);
    return (it != actionStates_.end()) ? it->second.wasPressedThisFrame : false;
}

bool InputManager::isActionReleased(InputAction action) const
{
    auto it = actionStates_.find(action);
    return (it != actionStates_.end()) ? it->second.wasReleasedThisFrame : false;
}