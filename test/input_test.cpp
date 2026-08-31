#include <cstdio>
#include <cstdlib>
#include <array>
#include <string_view>

#include "GL/glut.h"
#include "src/Input.hpp"

namespace
{
    // ============================================================
    // Two independent input managers
    // ============================================================
    InputManager keyboardInput;
    InputManager padInput;

    struct ActionMapping
    {
        InputAction action;
        std::string_view name;
    };

    constexpr std::array<ActionMapping, 10> ACTION_MAPPINGS = {{
        { InputAction::JUMP,        "JUMP" },
        { InputAction::CROUCH,      "CROUCH" },
        { InputAction::LEFT,        "LEFT" },
        { InputAction::RIGHT,       "RIGHT" },
        { InputAction::LIGHT_PUNCH, "LIGHT PUNCH" },
        { InputAction::LIGHT_KICK,  "LIGHT KICK" },
        { InputAction::HARD_PUNCH,  "HARD PUNCH" },
        { InputAction::HARD_KICK,   "HARD KICK" },
        { InputAction::BLOCK,       "BLOCK" },
        { InputAction::ENHANCE,     "ENHANCE" }
    }};
} // anonymous namespace

// ============================================================
// Display & Reshape
// ============================================================
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ============================================================
// Keyboard callbacks
// ============================================================
void keyDown(unsigned char key, [[maybe_unused]] int x, [[maybe_unused]] int y)
{
    handleKeyDown(key, x, y);

    // ESC
    if (key == 27)
    {
        std::printf("\nExiting input test...\n");
        shutdownGamepad();
        std::exit(0);
    }
}

void keyUp(unsigned char key, [[maybe_unused]] int x, [[maybe_unused]] int y)
{
    handleKeyUp(key, x, y);
}

// ============================================================
// Print action transitions
// ============================================================
void printActionState(
    std::string_view device,
    std::string_view actionName,
    const InputManager& input,
    InputAction action
)
{
    if (input.isActionPressed(action))
    {
        std::printf("[%s] Pressed:  %s\n", device.data(), actionName.data());
    }

    if (input.isActionReleased(action))
    {
        std::printf("[%s] Released: %s\n", device.data(), actionName.data());
    }
}

// ============================================================
// Update input
// ============================================================
void updateInput([[maybe_unused]] int value)
{
    // 1. Update SDL2 controller
    updateGamepad();

    // 2. Update InputManagers
    keyboardInput.update();
    padInput.update();

    // 3. Clear the shared queue - must happen after BOTH managers
    // above have had update() called, or whichever ran first would
    // consume every event before the other ever saw it.
    clearInputEvents();

    // 4. Print transitions
    for (const auto& mapping : ACTION_MAPPINGS)
    {
        printActionState("KEYBOARD", mapping.name, keyboardInput, mapping.action);
        printActionState("GAMEPAD", mapping.name, padInput, mapping.action);
    }

    // 5. Schedule next update
    glutTimerFunc(16, updateInput, 0);
}

// ============================================================
// Main
// ============================================================
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Input Manager Test");

    std::printf("============================================================\n");
    std::printf("                  INPUT MANAGER TEST\n");
    std::printf("============================================================\n\n");

    std::printf("Initializing SDL2 gamepad...\n");
    if (initGamepad())
    {
        std::printf("SDL2 gamepad initialized successfully.\n");
    }
    else
    {
        std::printf("No SDL2 gamepad available.\n");
        std::printf("Keyboard testing will still work.\n");
    }

    keyboardInput.applyKeyboardDefaults();
    padInput.applyPadDefaults();

    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, updateInput, 0);
    
    printf("\n");
    printf("============================================================\n");
    printf("KEYBOARD\n");
    printf("============================================================\n");

    printf("W       -> Jump\n");
    printf("S       -> Crouch\n");
    printf("D       -> Right\n");
    printf("A       -> Left\n");
    printf("K       -> LIGHT PUNCH\n");
    printf("L       -> LIGHT KICK\n");
    printf("J       -> HARD PUNCH\n");
    printf("I       -> HARD KICK\n");
    printf("SPACE   -> Block\n");
    printf("U       -> Enhance\n");


    printf("\n");
    printf("============================================================\n");
    printf("GAMEPAD\n");
    printf("============================================================\n");

    printf("Left Stick Up       -> Jump\n");
    printf("Left Stick Down     -> Crouch\n");
    printf("Left Stick Right    -> Right\n");
    printf("Left Stick Left     -> Left\n");

    printf("Cross               -> LIGHT PUNCH\n");
    printf("Circle              -> LIGHT KICK\n");
    printf("Square              -> HARD PUNCH\n");
    printf("Triangle            -> HARD KICK\n");

    printf("L1                  -> Block\n");
    printf("R1                  -> Enhance\n");

    printf("\n");
    printf("============================================================\n");
    printf("OUTPUT\n");
    printf("============================================================\n");

    printf("Pressed  = printed once when an action begins\n");
    printf("Released = printed once when an action ends\n");
    printf("ESC      = exit\n");

    printf("\n============================================================\n\n");

    glutMainLoop();
    return 0;
}