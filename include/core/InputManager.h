#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <unordered_map>

class InputManager
{
private:
    std::unordered_map<unsigned char, bool> currentKeyStates;
    std::unordered_map<unsigned char, bool> previousKeyStates;

    std::unordered_map<unsigned char, bool> currentSpecialKeyStates;
    std::unordered_map<unsigned char, bool> previousSpecialKeyStates;
public:
    InputManager() = default;

    void updateKeyState();

    bool isKeyDown(unsigned char key) const;
    bool isKeyPressed(unsigned char key) const;
    bool isKeyReleased(unsigned char key) const;

    bool isSpecialKeyDown(unsigned char key) const;
    bool isSpecialKeyPressed(unsigned char key) const;
    bool isSpecialKeyReleased(unsigned char key) const;

    // GLUT callback handlers
    void handleKeyDown(unsigned char key, int x, int y);
    void handleKeyUp(unsigned char key, int x, int y);
    void handleSpecialKeyDown(int key, int x, int y);
    void handleSpecialKeyUp(int key, int x, int y);

    /*
        Must implement differing position for special keys so that they don't collide with the normal ones
    */
};
#endif  // INPUT_MANAGER_H