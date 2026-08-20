#ifndef INPUT_HPP
#define INPUT_HPP

#include <unordered_map>
#include <functional>
#include "GL/glut.h"

#include <stdio.h>

enum class InputAction
{
    JUMP,
    CROUCH,
    FORWARD,
    BACKWARD,
    CROSS,
    CIRCLE,
    SQUARE,
    TRIANGLE,
    BLOCK,
    ENHANCE
};

enum class KeyCode  // Actual key code the game supports
{
    // Player 1 default keys
    W, S, D, A,
    K, L, J, I,
    Space, Shift,

    // Player 2 default keys - arrows are GLUT "special" keys, handled
    // through a separate callback pair (see handleSpecialKeyDown/Up)
    Up, Down, Left, Right,
    Num1, Num2, Num3, Num4, Num5, Num6
};

enum SpecialIndex 
{ 
    SPECIAL_UP = 0, 
    SPECIAL_DOWN = 1, 
    SPECIAL_LEFT = 2, 
    SPECIAL_RIGHT = 3 
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
    // No hardcoded bindings in the constructor anymore - two instances
    // with identical defaults would collide on the same physical keys.
    // Call applyPlayer1Defaults() / applyPlayer2Defaults() explicitly,
    // or setBinding() individually for custom control schemes.
    InputManager() = default;

    void setBinding(InputAction action, KeyCode key);
    void update(const std::function<bool(KeyCode)>& isRawKeyDown);

    bool isActionHeld(InputAction action) const;
    bool isActionPressed(InputAction action) const;
    bool isActionReleased(InputAction action) const;

    void applyPlayer1Defaults(); // WASD + KLJI + Space + Shift
    void applyPlayer2Defaults(); // Arrows + 1234 + 5 + 6

private:
    std::unordered_map<InputAction, KeyCode> bindings_;
    std::unordered_map<InputAction, InputState> actionStates_;
};

// GLUT Static Input Callbacks - ASCII keys
void handleKeyDown(unsigned char key, int x, int y);
void handleKeyUp(unsigned char key, int x, int y);

// GLUT Static Input Callbacks - non-ASCII "special" keys (arrows etc.)
void handleSpecialKeyDown(int key, int x, int y);
void handleSpecialKeyUp(int key, int x, int y);

bool isGlutKeyDown(KeyCode code);

#endif // INPUT_HPP