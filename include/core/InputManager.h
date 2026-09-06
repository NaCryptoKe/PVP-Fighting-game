#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <array>

constexpr int MAX_KEYS = 256;
constexpr int MAX_SPECIAL_KEYS = 256;

class InputManager
{
private:
    // Fixed arrays eliminate hash-map overhead and prevent dynamic allocation
    std::array<bool, MAX_KEYS> currentKeyStates{};
    std::array<bool, MAX_KEYS> previousKeyStates{};

    std::array<bool, MAX_SPECIAL_KEYS> currentSpecialKeyStates{};
    std::array<bool, MAX_SPECIAL_KEYS> previousSpecialKeyStates{};

public:
    InputManager() = default;

    // Call once per frame (e.g., at the beginning of display/update loop)
    void updateKeyState();

    // Standard ASCII keys
    bool isKeyDown(unsigned char key) const;
    bool isKeyPressed(unsigned char key) const;
    bool isKeyReleased(unsigned char key) const;

    // GLUT Special keys (GLUT_KEY_*)
    bool isSpecialKeyDown(int key) const;
    bool isSpecialKeyPressed(int key) const;
    bool isSpecialKeyReleased(int key) const;

    // GLUT callback handlers
    void handleKeyDown(unsigned char key, int x, int y);
    void handleKeyUp(unsigned char key, int x, int y);
    void handleSpecialKeyDown(int key, int x, int y);
    void handleSpecialKeyUp(int key, int x, int y);
};

#endif // INPUT_MANAGER_H