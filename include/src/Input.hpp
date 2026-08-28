#ifndef INPUT_HPP
#define INPUT_HPP

#include <unordered_map>
#include <functional>

#include "GL/glut.h"
#include <SDL2/SDL.h>

struct InputEvent
{
    KeyCode key;
    bool pressed;
};

enum class InputAction
{
    JUMP,
    CROUCH,
    FORWARD,
    BACKWARD,
    LIGHT_PUNCH,
    LIGHT_KICK,
    HARD_PUNCH,
    HARD_KICK,
    BLOCK,
    ENHANCE
};

enum class KeyCode
{
    // keyboard
    W, S, D, A,
    K, L, J, I,
    Space, U,

    // SDL2 Gamepad
    PadCross,
    PadCircle,
    PadSquare,
    PadTriangle,

    PadL1,
    PadR1,
    PadL2,
    PadR2,
    PadL3,
    PadR3,

    PadStickUp,
    PadStickDown,
    PadStickLeft,
    PadStickRight,

    PadDPADUp,
    PadDPADDown,
    PadDPADLeft,
    PadDPADRight
};

struct InputState
{
    bool isHeld = false;
    bool wasPressedThisFrame = false;
    bool wasReleasedThisFrame = false;
};

class InputManager
{
public:
    InputManager() = default;

    void setBinding(InputAction action, KeyCode key);

    void update(const std::function<bool(KeyCode)>& isRawKeyDown);

    bool isActionHeld(InputAction action) const;
    bool isActionPressed(InputAction action) const;
    bool isActionReleased(InputAction action) const;

    void applyKeyboardDefaults();
    void applyPadDefaults();
    
private: 
    std::unordered_map<InputAction, KeyCode> bindings;
    std::unordered_map<InputAction, InputState> actionStates;
};

// ============================================================
// GLUT Keyboard Input
// ============================================================

void handleKeyDown(unsigned char key, int x, int y);
void handleKeyUp(unsigned char key, int x, int y);

bool isGlutKeyDown(KeyCode code);

// ============================================================
// SDL2 Gamepad Input
// ============================================================

// Initialize SDL2's gamepad subsystem and connect to the first
// available controller.
bool initGamepad();

// Close the currently connected gamepad and shut down SDL2.
void shutdownGamepad();

// Poll SDL2 events. Call this once per game frame.
void updateGamepad();

// Returns whether a particular SDL2 KeyCode is currently held.
bool isGamepadKeyDown(KeyCode code);

#endif // INPUT_HPP