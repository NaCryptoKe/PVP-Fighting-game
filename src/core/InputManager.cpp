#include "core/InputManager.h"

void InputManager::updateKeyState()
{
    previousKeyStates = currentKeyStates;
    previousSpecialKeyStates = currentSpecialKeyStates; 
}

bool InputManager::isKeyDown(unsigned char key) const
{
    auto it = currentKeyStates.find(key);

    return (it != currentKeyStates.end()) ? it->second : false;
}

bool InputManager::isKeyPressed(unsigned char key) const
{
    bool isDownNow = isKeyDown(key);
    auto it = previousKeyStates.find(key);
    bool wasDownLastFrame = ( it != previousKeyStates.end()) ? it->second : false;

    return isDownNow && !wasDownLastFrame;
}

bool InputManager::isKeyReleased(unsigned char key) const
{
    bool isDownNow = isKeyDown(key);
    auto it = previousKeyStates.find(key);
    bool wasDownLastFrame = ( it != previousKeyStates.end()) ? it->second : false;

    return !isDownNow && wasDownLastFrame;
}

void InputManager::handleKeyDown(unsigned char key, int x, int y) {
    currentKeyStates[key] = true;
}

void InputManager::handleKeyUp(unsigned char key, int x, int y) {
    currentKeyStates[key] = false;
}

bool InputManager::isSpecialKeyDown(int key) const
{
    auto it = currentSpecialKeyStates.find(key);

    return (it != currentKeyStates.end()) ? it->second : false;
}
bool InputManager::isSpecialKeyPressed(int key) const
{
    bool isDownNow = isSpecialKeyDown(key);
    auto it = previousSpecialKeyStates.find(key);
    bool wasDownLastFrame = ( it != previousSpecialKeyStates.end()) ? it->second : false;

    return isDownNow && !wasDownLastFrame;
}
bool InputManager::isSpecialKeyReleased(int key) const
{
    bool isDownNow = isSpecialKeyDown(key);
    auto it = previousSpecialKeyStates.find(key);
    bool wasDownLastFrame = ( it != previousSpecialKeyStates.end()) ? it->second : false;

    return !isDownNow && wasDownLastFrame;
}
void InputManager::handleSpecialKeyDown(int key, int x, int y) {
    currentSpecialKeyStates[key] = true;
}
void InputManager::handleSpecialKeyUp(int key, int x, int y) {
    currentSpecialKeyStates[key] = false;
}