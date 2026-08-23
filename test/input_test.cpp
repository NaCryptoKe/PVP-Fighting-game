#include <stdio.h>
#include <stdlib.h>

#include "GL/glut.h"

#include "src/Input.hpp"

// ============================================================
// Two independent input managers
// ============================================================

InputManager keyboardInput;
InputManager padInput;


// ============================================================
// Display
// ============================================================

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glutSwapBuffers();
}


// ============================================================
// Reshape
// ============================================================

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(
        0.0f, 800.0f,
        0.0f, 600.0f,
        -1.0f, 1.0f
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


// ============================================================
// Keyboard callbacks
// ============================================================

void keyDown(unsigned char key, int x, int y)
{
    handleKeyDown(key, x, y);

    // ESC
    if (key == 27)
    {
        printf("\nExiting input test...\n");

        shutdownGamepad();

        exit(0);
    }
}

void keyUp(unsigned char key, int x, int y)
{
    handleKeyUp(key, x, y);
}


// ============================================================
// Print action transitions
// ============================================================

void printActionState(
    const char* device,
    const char* actionName,
    InputManager& input,
    InputAction action
)
{
    if (input.isActionPressed(action))
    {
        printf(
            "[%s] Pressed:  %s\n",
            device,
            actionName
        );
    }

    if (input.isActionReleased(action))
    {
        printf(
            "[%s] Released: %s\n",
            device,
            actionName
        );
    }
}


// ============================================================
// Update input
// ============================================================

void updateInput(int value)
{
    (void)value;

    // ========================================================
    // 1. Update SDL2 controller
    // ========================================================

    updateGamepad();


    // ========================================================
    // 2. Update keyboard InputManager
    // ========================================================

    keyboardInput.update(
        [](KeyCode key)
        {
            return isGlutKeyDown(key);
        }
    );


    // ========================================================
    // 3. Update gamepad InputManager
    // ========================================================

    padInput.update(
        [](KeyCode key)
        {
            return isGamepadKeyDown(key);
        }
    );


    // ========================================================
    // Actions to monitor
    // ========================================================

    const InputAction actions[] =
    {
        InputAction::JUMP,
        InputAction::CROUCH,
        InputAction::FORWARD,
        InputAction::BACKWARD,
        InputAction::LIGHT_PUNCH,
        InputAction::LIGHT_KICK,
        InputAction::HARD_PUNCH,
        InputAction::HARD_KICK,
        InputAction::BLOCK,
        InputAction::ENHANCE
    };

    const char* names[] =
    {
        "JUMP",
        "CROUCH",
        "FORWARD",
        "BACKWARD",
        "LIGHT PUNCH",
        "LIGHT KICK",
        "HARD PUNCH",
        "HARD KICK",
        "BLOCK",
        "ENHANCE"
    };


    // ========================================================
    // 4. Print keyboard transitions
    // ========================================================

    for (int i = 0; i < 10; ++i)
    {
        printActionState(
            "KEYBOARD",
            names[i],
            keyboardInput,
            actions[i]
        );
    }


    // ========================================================
    // 5. Print gamepad transitions
    // ========================================================

    for (int i = 0; i < 10; ++i)
    {
        printActionState(
            "GAMEPAD",
            names[i],
            padInput,
            actions[i]
        );
    }


    // ========================================================
    // 6. Schedule next update
    // ========================================================

    glutTimerFunc(16, updateInput, 0);
}


// ============================================================
// Main
// ============================================================

int main(int argc, char** argv)
{
    // ========================================================
    // 1. Initialize GLUT
    // ========================================================

    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGBA
    );

    glutInitWindowSize(800, 600);

    glutCreateWindow("Input Manager Test");


    // ========================================================
    // 2. Initialize SDL2 Gamepad
    // ========================================================

    printf("============================================================\n");
    printf("                  INPUT MANAGER TEST\n");
    printf("============================================================\n\n");

    printf("Initializing SDL2 gamepad...\n");

    if (initGamepad())
    {
        printf("SDL2 gamepad initialized successfully.\n");
    }
    else
    {
        printf("No SDL2 gamepad available.\n");
        printf("Keyboard testing will still work.\n");
    }


    // ========================================================
    // 3. Apply control schemes
    // ========================================================

    keyboardInput.applyKeyboardDefaults();
    padInput.applyPadDefaults();


    // ========================================================
    // 4. Register keyboard callbacks
    // ========================================================

    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);


    // ========================================================
    // 5. Register rendering callbacks
    // ========================================================

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);


    // ========================================================
    // 6. Start input update loop
    // ========================================================

    glutTimerFunc(16, updateInput, 0);


    // ========================================================
    // 7. Instructions
    // ========================================================

    printf("\n");
    printf("============================================================\n");
    printf("KEYBOARD\n");
    printf("============================================================\n");

    printf("W       -> Jump\n");
    printf("S       -> Crouch\n");
    printf("D       -> Forward\n");
    printf("A       -> Backward\n");
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
    printf("Left Stick Right    -> Forward\n");
    printf("Left Stick Left     -> Backward\n");

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


    // ========================================================
    // 8. Start GLUT event loop
    // ========================================================

    glutMainLoop();

    return 0;
}