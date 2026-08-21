#include "src/Input.hpp"
#include <cctype>

static bool g_keys[256] = { false };        // ASCII keys, indexed by lowercase char
                                            // Basically a LUT
static bool g_shiftDown = false;
static bool g_specialKeys[4] = { false, false, false, false };

// PS4 related
static unsigned int g_padButtons = 0;
static int g_padAxisX = 0;
static int g_padAxisY = 0;

static const int PAD_STICK_DEADZONE = 300;

static const unsigned int PAD_BTN_CROSS    = 1u << 0;
static const unsigned int PAD_BTN_CIRCLE   = 1u << 1;
static const unsigned int PAD_BTN_TRIANGLE = 1u << 2;
static const unsigned int PAD_BTN_SQUARE   = 1u << 3;
static const unsigned int PAD_BTN_L1       = 1u << 4;
static const unsigned int PAD_BTN_R1       = 1u << 5;
static const unsigned int PAD_BTN_L2       = 1u << 6;
static const unsigned int PAD_BTN_R2       = 1u << 7;
static const unsigned int PAD_BTN_SHARE    = 1u << 8;
static const unsigned int PAD_BTN_OPTION   = 1u << 9;
static const unsigned int PAD_BTN_PS       = 1u << 10;
static const unsigned int PAD_BTN_L3       = 1u << 11;
static const unsigned int PAD_BTN_R3       = 1u << 12;

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

// Takes keycode so that, and returns bool state of current state
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

        case KeyCode::PadCross:    return (g_padButtons & PAD_BTN_CROSS)    != 0;
        case KeyCode::PadCircle:   return (g_padButtons & PAD_BTN_CIRCLE)   != 0;
        case KeyCode::PadSquare:   return (g_padButtons & PAD_BTN_SQUARE)   != 0;
        case KeyCode::PadTriangle: return (g_padButtons & PAD_BTN_TRIANGLE) != 0;
        case KeyCode::PadL1:       return (g_padButtons & PAD_BTN_L1)       != 0;
        case KeyCode::PadR1:       return (g_padButtons & PAD_BTN_R1)       != 0;

        case KeyCode::PadStickUp:    return g_padAxisY >  PAD_STICK_DEADZONE;
        case KeyCode::PadStickDown:  return g_padAxisY < -PAD_STICK_DEADZONE;
        case KeyCode::PadStickLeft:  return g_padAxisX < -PAD_STICK_DEADZONE;
        case KeyCode::PadStickRight: return g_padAxisX >  PAD_STICK_DEADZONE;

        default: return false;
    }
}

// Creating a hasmap for bindings_
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

void InputManager::applyPadDefaults()
{
    setBinding(InputAction::JUMP,     KeyCode::PadStickUp);
    setBinding(InputAction::CROUCH,   KeyCode::PadStickDown);
    setBinding(InputAction::FORWARD,  KeyCode::PadStickRight);
    setBinding(InputAction::BACKWARD, KeyCode::PadStickLeft);
    setBinding(InputAction::CROSS,    KeyCode::PadCross);
    setBinding(InputAction::CIRCLE,   KeyCode::PadCircle);
    setBinding(InputAction::SQUARE,   KeyCode::PadSquare);
    setBinding(InputAction::TRIANGLE, KeyCode::PadTriangle);
    setBinding(InputAction::BLOCK,    KeyCode::PadL1);
    setBinding(InputAction::ENHANCE,  KeyCode::PadR1);
}

void updatePadState(unsigned int buttonMask, int x, int y)
{
    g_padButtons = buttonMask;
    g_padAxisX = x;
    g_padAxisY = y;
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