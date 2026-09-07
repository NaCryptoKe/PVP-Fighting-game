#include "core/InputManager.h"

void InputManager::updateKeyState()
{
    previousKeyStates = currentKeyStates;
    previousSpecialKeyStates = currentSpecialKeyStates; 
}

// STANDARD KEYS

bool InputManager::isKeyDown(unsigned char key) const
{
    return currentKeyStates[key];
}

bool InputManager::isKeyPressed(unsigned char key) const
{
    return currentKeyStates[key] && !previousKeyStates[key];
}

bool InputManager::isKeyReleased(unsigned char key) const
{
    return !currentKeyStates[key] && previousKeyStates[key];
}

void InputManager::handleKeyDown(unsigned char key, [[maybe_unused]]int x, [[maybe_unused]]int y)
{
    currentKeyStates[key] = true;
}

void InputManager::handleKeyUp(unsigned char key, [[maybe_unused]]int x, [[maybe_unused]]int y)
{
    currentKeyStates[key] = false;
}

// SPECIAL KEYS

bool InputManager::isSpecialKeyDown(int key) const
{
    return (key >= 0 && key < MAX_SPECIAL_KEYS) ? currentSpecialKeyStates[key] : false;
}

bool InputManager::isSpecialKeyPressed(int key) const
{
    if (key < 0 || key >= MAX_SPECIAL_KEYS) return false;
    return currentSpecialKeyStates[key] && !previousSpecialKeyStates[key];
}

bool InputManager::isSpecialKeyReleased(int key) const
{
    if (key < 0 || key >= MAX_SPECIAL_KEYS) return false;
    return !currentSpecialKeyStates[key] && previousSpecialKeyStates[key];
}

void InputManager::handleSpecialKeyDown(int key, [[maybe_unused]]int x, [[maybe_unused]]int y)
{
    if (key >= 0 && key < MAX_SPECIAL_KEYS) {
        currentSpecialKeyStates[key] = true;
    }
}

void InputManager::handleSpecialKeyUp(int key, [[maybe_unused]]int x, [[maybe_unused]]int y)
{
    if (key >= 0 && key < MAX_SPECIAL_KEYS) {
        currentSpecialKeyStates[key] = false;
    }
}