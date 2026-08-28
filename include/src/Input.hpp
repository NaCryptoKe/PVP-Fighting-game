#ifndef INPUT_HPP
#define INPUT_HPP

#include <unordered_map>
#include <functional>

#include "GL/glut.h"
#include <SDL2/SDL.h>

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
    PadDPADRight,

    COUNT
};

struct InputEvent
{
    KeyCode key;
    bool pressed;
};

// ============================================================
// Input Event Queue
// ============================================================
//
// Raw devices (GLUT keyboard callbacks, etc.) push discrete
// press/release events here; InputManager::update() drains the
// queue once per frame. Exposed publicly so:
//   - new input backends can feed events without touching Input.cpp
//   - tests can inject synthetic events without a live window
//
// The queue is a fixed-size ring buffer (see INPUT_BUFFER_SIZE in
// Input.cpp). If update() isn't called often enough relative to the
// event rate, older un-drained events can cause new ones to be
// dropped - use takeDroppedInputEventCount() to detect this.

void pushInputEvent(KeyCode key, bool pressed);

// Returns how many events have been dropped due to a full buffer
// since the last call, then resets the counter to 0.
int takeDroppedInputEventCount();

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

    void update();

    bool isActionHeld(InputAction action) const;
    bool isActionPressed(InputAction action) const;
    bool isActionReleased(InputAction action) const;

    void applyKeyboardDefaults();
    void applyPadDefaults();
    
private: 
    bool keyState[static_cast<int>(KeyCode::COUNT)];
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