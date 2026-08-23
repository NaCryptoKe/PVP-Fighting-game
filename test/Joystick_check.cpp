/*
 * Run using:
 * g++ -std=c++17 -Wall -Wextra -Wpedantic test/Joystick_check.cpp -o joystick -lSDL2 -lGL -lGLU -lglut && ./joystick
*/

#include <stdio.h>
#include <stdlib.h>

#include "GL/glut.h"
#include <SDL2/SDL.h>

// ============================================================
// SDL2 Controller
// ============================================================

static SDL_GameController* controller = nullptr;

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
// Print controller button
// ============================================================

void printButton(SDL_GameControllerButton button)
{
    const char* name =
        SDL_GameControllerGetStringForButton(button);

    printf(
        "[GAMEPAD] Button pressed: %s\n",
        name
    );
}

// ============================================================
// Print controller axis
// ============================================================

void printAxis(
    SDL_GameControllerAxis axis,
    Sint16 value
)
{
    const char* name =
        SDL_GameControllerGetStringForAxis(axis);

    printf(
        "[GAMEPAD] Axis: %s = %d\n",
        name,
        value
    );
}

// ============================================================
// Poll SDL2
// ============================================================

void updateGamepadTest(int value)
{
    (void)value;

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            // ==================================================
            // Controller button
            // ==================================================

            case SDL_CONTROLLERBUTTONDOWN:
            {
                printButton(
                    static_cast<SDL_GameControllerButton>(
                        event.cbutton.button
                    )
                );

                break;
            }

            // ==================================================
            // Controller button released
            // ==================================================

            case SDL_CONTROLLERBUTTONUP:
            {
                const char* name =
                    SDL_GameControllerGetStringForButton(
                        static_cast<SDL_GameControllerButton>(
                            event.cbutton.button
                        )
                    );

                printf(
                    "[GAMEPAD] Button released: %s\n",
                    name
                );

                break;
            }

            // ==================================================
            // Controller axis
            // ==================================================

            case SDL_CONTROLLERAXISMOTION:
            {
                // Ignore tiny movements caused by controller noise.
                if (event.caxis.value > 3000 ||
                    event.caxis.value < -3000)
                {
                    printAxis(
                        static_cast<SDL_GameControllerAxis>(
                            event.caxis.axis
                        ),
                        event.caxis.value
                    );
                }

                break;
            }

            // ==================================================
            // Controller connected
            // ==================================================

            case SDL_CONTROLLERDEVICEADDED:
            {
                printf(
                    "[GAMEPAD] Controller connected.\n"
                );

                if (controller == nullptr)
                {
                    if (SDL_IsGameController(
                            event.cdevice.which))
                    {
                        controller =
                            SDL_GameControllerOpen(
                                event.cdevice.which
                            );

                        if (controller)
                        {
                            printf(
                                "[GAMEPAD] Name: %s\n",
                                SDL_GameControllerName(
                                    controller
                                )
                            );
                        }
                    }
                }

                break;
            }

            // ==================================================
            // Controller disconnected
            // ==================================================

            case SDL_CONTROLLERDEVICEREMOVED:
            {
                printf(
                    "[GAMEPAD] Controller disconnected.\n"
                );

                if (controller)
                {
                    SDL_GameControllerClose(controller);
                    controller = nullptr;
                }

                break;
            }

            default:
                break;
        }
    }

    glutTimerFunc(
        16,
        updateGamepadTest,
        0
    );
}

// ============================================================
// Keyboard
// ============================================================

void keyDown(
    unsigned char key,
    int x,
    int y
)
{
    (void)x;
    (void)y;

    if (key == 27)
    {
        printf("\nExiting...\n");

        if (controller)
        {
            SDL_GameControllerClose(controller);
            controller = nullptr;
        }

        SDL_Quit();

        exit(0);
    }
}

// ============================================================
// Initialize SDL2
// ============================================================

bool initializeSDL()
{
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf(
            "SDL_Init failed: %s\n",
            SDL_GetError()
        );

        return false;
    }

    printf(
        "SDL initialized successfully.\n"
    );

    int joystickCount = SDL_NumJoysticks();

    printf(
        "SDL detected %d joystick(s).\n",
        joystickCount
    );

    // ========================================================
    // Find first compatible controller
    // ========================================================

    for (int i = 0; i < joystickCount; ++i)
    {
        printf(
            "Joystick %d: %s\n",
            i,
            SDL_JoystickNameForIndex(i)
        );

        if (!SDL_IsGameController(i))
        {
            printf(
                "  Not recognized as SDL GameController.\n"
            );

            continue;
        }

        controller =
            SDL_GameControllerOpen(i);

        if (controller)
        {
            printf(
                "\nController opened successfully!\n"
            );

            printf(
                "Controller name: %s\n",
                SDL_GameControllerName(controller)
            );

            return true;
        }

        printf(
            "  Failed to open: %s\n",
            SDL_GetError()
        );
    }

    return false;
}

// ============================================================
// Main
// ============================================================

int main(
    int argc,
    char** argv
)
{
    // ========================================================
    // GLUT
    // ========================================================

    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGBA
    );

    glutInitWindowSize(
        800,
        600
    );

    glutCreateWindow(
        "SDL2 Gamepad Test"
    );

    // ========================================================
    // SDL2
    // ========================================================

    printf(
        "============================================================\n"
    );

    printf(
        "                 SDL2 GAMEPAD TEST\n"
    );

    printf(
        "============================================================\n\n"
    );

    bool initialized =
        initializeSDL();

    if (!initialized)
    {
        printf(
            "\nNo compatible SDL2 gamepad found.\n"
        );
    }

    // ========================================================
    // Keyboard
    // ========================================================

    glutKeyboardFunc(keyDown);

    // ========================================================
    // Rendering
    // ========================================================

    glutDisplayFunc(display);

    glutReshapeFunc(reshape);

    // ========================================================
    // Start polling
    // ========================================================

    glutTimerFunc(
        16,
        updateGamepadTest,
        0
    );

    // ========================================================
    // Instructions
    // ========================================================

    printf("\n");
    printf(
        "============================================================\n"
    );

    printf(
        "Move / press ANY controller input.\n"
    );

    printf(
        "Buttons will print their SDL2 name.\n"
    );

    printf(
        "Axes will print their value.\n"
    );

    printf(
        "ESC -> Exit\n"
    );

    printf(
        "============================================================\n\n"
    );

    // ========================================================
    // Main loop
    // ========================================================

    glutMainLoop();

    return 0;
}