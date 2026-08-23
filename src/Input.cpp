#include "src/Input.hpp"

#include <cctype>
#include <cstdio>

// ============================================================
// GLUT Keyboard State
// ============================================================

static bool g_keys[256] = { false };

void handleKeyDown(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    key = static_cast<unsigned char>(
        std::tolower(static_cast<unsigned char>(key))
    );

    g_keys[key] = true;
}

void handleKeyUp(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    key = static_cast<unsigned char>(
        std::tolower(static_cast<unsigned char>(key))
    );

    g_keys[key] = false;
}

// ============================================================
// GLUT Keyboard Query
// ============================================================

bool isGlutKeyDown(KeyCode code)
{
    switch (code)
    {
        case KeyCode::W:
            return g_keys['w'];

        case KeyCode::S:
            return g_keys['s'];

        case KeyCode::D:
            return g_keys['d'];

        case KeyCode::A:
            return g_keys['a'];

        case KeyCode::K:
            return g_keys['k'];

        case KeyCode::L:
            return g_keys['l'];

        case KeyCode::J:
            return g_keys['j'];

        case KeyCode::I:
            return g_keys['i'];

        case KeyCode::Space:
            return g_keys[' '];

        case KeyCode::U:
            return g_keys['u'];

        default:
            return false;
    }
}

// ============================================================
// SDL2 Gamepad State
// ============================================================

static SDL_GameController* g_controller = nullptr;

// Deadzone for analog sticks.
//
// SDL2 stick values normally range approximately from:
//     -32768 -> full left/up
//      0     -> center
//     32767 -> full right/down
//
// We use a deadzone so tiny stick movements don't count as input.
static constexpr Sint16 GAMEPAD_DEADZONE = 8000;

// ============================================================
// Initialize Gamepad
// ============================================================

bool initGamepad()
{
    // Initialize SDL2's game-controller subsystem.
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0)
    {
        std::printf(
            "SDL2: Failed to initialize game controller subsystem: %s\n",
            SDL_GetError()
        );

        return false;
    }

    std::printf(
        "SDL2: Found %d joystick(s).\n",
        SDL_NumJoysticks()
    );

    // Look through all connected devices.
    for (int i = 0; i < SDL_NumJoysticks(); ++i)
    {
        // We specifically want a device SDL recognizes as a game
        // controller rather than an arbitrary joystick.
        if (!SDL_IsGameController(i))
        {
            continue;
        }

        g_controller = SDL_GameControllerOpen(i);

        if (!g_controller)
        {
            std::printf(
                "SDL2: Failed to open controller %d: %s\n",
                i,
                SDL_GetError()
            );

            continue;
        }

        std::printf(
            "SDL2: Controller connected: %s\n",
            SDL_GameControllerName(g_controller)
        );

        return true;
    }

    std::printf("SDL2: No compatible game controller found.\n");

    return false;
}

// ============================================================
// Shutdown Gamepad
// ============================================================

void shutdownGamepad()
{
    if (g_controller != nullptr)
    {
        SDL_GameControllerClose(g_controller);
        g_controller = nullptr;
    }

    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
}

// ============================================================
// Update Gamepad
// ============================================================

void updateGamepad()
{
    if (g_controller == nullptr)
    {
        return;
    }

    SDL_Event event;

    // Process all pending SDL events.
    //
    // We don't directly change our input state here.
    // SDL maintains the current controller state for us.
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_CONTROLLERDEVICEADDED:
            {
                // If we don't currently have a controller,
                // try opening the newly connected one.
                if (g_controller == nullptr)
                {
                    if (SDL_IsGameController(event.cdevice.which))
                    {
                        g_controller =
                            SDL_GameControllerOpen(event.cdevice.which);

                        if (g_controller)
                        {
                            std::printf(
                                "SDL2: Controller connected: %s\n",
                                SDL_GameControllerName(g_controller)
                            );
                        }
                    }
                }

                break;
            }

            case SDL_CONTROLLERDEVICEREMOVED:
            {
                SDL_JoystickID removedID =
                    event.cdevice.which;

                SDL_Joystick* joystick =
                    SDL_GameControllerGetJoystick(g_controller);

                if (joystick != nullptr)
                {
                    SDL_JoystickID currentID =
                        SDL_JoystickInstanceID(joystick);

                    if (currentID == removedID)
                    {
                        std::printf(
                            "SDL2: Controller disconnected.\n"
                        );

                        SDL_GameControllerClose(g_controller);
                        g_controller = nullptr;
                    }
                }

                break;
            }

            default:
                break;
        }
    }

    // Refresh SDL's internal controller state.
    SDL_GameControllerUpdate();
}

// ============================================================
// SDL2 Gamepad Query
// ============================================================

bool isGamepadKeyDown(KeyCode code)
{
    if (g_controller == nullptr)
    {
        return false;
    }

    switch (code)
    {
        // ----------------------------------------------------
        // Face buttons
        // ----------------------------------------------------

        case KeyCode::PadCross:
            return SDL_GameControllerGetButton(
                g_controller,
                SDL_CONTROLLER_BUTTON_A
            );

        case KeyCode::PadCircle:
            return SDL_GameControllerGetButton(
                g_controller,
                SDL_CONTROLLER_BUTTON_B
            );

        case KeyCode::PadSquare:
            return SDL_GameControllerGetButton(
                g_controller,
                SDL_CONTROLLER_BUTTON_X
            );

        case KeyCode::PadTriangle:
            return SDL_GameControllerGetButton(
                g_controller,
                SDL_CONTROLLER_BUTTON_Y
            );

        // ----------------------------------------------------
        // Shoulder buttons
        // ----------------------------------------------------

        case KeyCode::PadL1:
            return SDL_GameControllerGetButton(
                g_controller,
                SDL_CONTROLLER_BUTTON_LEFTSHOULDER
            );

        case KeyCode::PadR1:
            return SDL_GameControllerGetButton(
                g_controller,
                SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
            );

        
        // ----------------------------------------------------
        // L2 / R2
        // ----------------------------------------------------

        case KeyCode::PadL2:
        {
            Sint16 value =
                SDL_GameControllerGetAxis(
                    g_controller,
                    SDL_CONTROLLER_AXIS_TRIGGERLEFT
                );

            return value > GAMEPAD_DEADZONE;
        }

        case KeyCode::PadR2:
        {
            Sint16 value =
                SDL_GameControllerGetAxis(
                    g_controller,
                    SDL_CONTROLLER_AXIS_TRIGGERRIGHT
                );

            return value > GAMEPAD_DEADZONE;
        }

        // ----------------------------------------------------
        // L3 / R3
        // ----------------------------------------------------

        case KeyCode::PadL3:
            return SDL_GameControllerGetButton(
                g_controller,
                SDL_CONTROLLER_BUTTON_LEFTSTICK
            );

        case KeyCode::PadR3:
            return SDL_GameControllerGetButton(
                g_controller,
                SDL_CONTROLLER_BUTTON_RIGHTSTICK
            );

        // ----------------------------------------------------
        // Left analog stick
        // ----------------------------------------------------

        case KeyCode::PadStickLeft:
        {
            Sint16 x =
                SDL_GameControllerGetAxis(
                    g_controller,
                    SDL_CONTROLLER_AXIS_LEFTX
                );

            return x < -GAMEPAD_DEADZONE;
        }

        case KeyCode::PadStickRight:
        {
            Sint16 x =
                SDL_GameControllerGetAxis(
                    g_controller,
                    SDL_CONTROLLER_AXIS_LEFTX
                );

            return x > GAMEPAD_DEADZONE;
        }

        case KeyCode::PadStickUp:
        {
            Sint16 y =
                SDL_GameControllerGetAxis(
                    g_controller,
                    SDL_CONTROLLER_AXIS_LEFTY
                );

            // SDL's positive Y is normally DOWN.
            // Therefore negative Y means UP.
            return y < -GAMEPAD_DEADZONE;
        }

        case KeyCode::PadStickDown:
        {
            Sint16 y =
                SDL_GameControllerGetAxis(
                    g_controller,
                    SDL_CONTROLLER_AXIS_LEFTY
                );

            return y > GAMEPAD_DEADZONE;
        }

        // ----------------------------------------------------
        // D-pad
        // ----------------------------------------------------

        case KeyCode::PadDPADUp:
            return SDL_GameControllerGetButton(
                g_controller,
                SDL_CONTROLLER_BUTTON_DPAD_UP
            );

        case KeyCode::PadDPADDown:
            return SDL_GameControllerGetButton(
                g_controller,
                SDL_CONTROLLER_BUTTON_DPAD_DOWN
            );

        case KeyCode::PadDPADLeft:
            return SDL_GameControllerGetButton(
                g_controller,
                SDL_CONTROLLER_BUTTON_DPAD_LEFT
            );

        case KeyCode::PadDPADRight:
            return SDL_GameControllerGetButton(
                g_controller,
                SDL_CONTROLLER_BUTTON_DPAD_RIGHT
            );

        // ----------------------------------------------------
        // Keyboard keys are not handled here.
        // ----------------------------------------------------

        default:
            return false;
    }
}

// ============================================================
// InputManager
// ============================================================

void InputManager::setBinding(InputAction action, KeyCode key)
{
    bindings[action] = key;
    actionStates[action] = InputState{};
}

// ============================================================
// Keyboard Defaults
// ============================================================

void InputManager::applyKeyboardDefaults()
{
    setBinding(InputAction::JUMP,        KeyCode::W);
    setBinding(InputAction::CROUCH,      KeyCode::S);
    setBinding(InputAction::FORWARD,     KeyCode::D);
    setBinding(InputAction::BACKWARD,    KeyCode::A);

    setBinding(InputAction::LIGHT_PUNCH, KeyCode::K);
    setBinding(InputAction::LIGHT_KICK,  KeyCode::L);
    setBinding(InputAction::HARD_PUNCH,  KeyCode::J);
    setBinding(InputAction::HARD_KICK,   KeyCode::I);

    setBinding(InputAction::BLOCK,       KeyCode::Space);
    setBinding(InputAction::ENHANCE,     KeyCode::U);
}

// ============================================================
// Gamepad Defaults
// ============================================================

void InputManager::applyPadDefaults()
{
    setBinding(
        InputAction::JUMP,
        KeyCode::PadStickUp
    );

    setBinding(
        InputAction::CROUCH,
        KeyCode::PadStickDown
    );

    setBinding(
        InputAction::FORWARD,
        KeyCode::PadStickRight
    );

    setBinding(
        InputAction::BACKWARD,
        KeyCode::PadStickLeft
    );

    setBinding(
        InputAction::LIGHT_PUNCH,
        KeyCode::PadCross
    );

    setBinding(
        InputAction::LIGHT_KICK,
        KeyCode::PadCircle
    );

    setBinding(
        InputAction::HARD_PUNCH,
        KeyCode::PadSquare
    );

    setBinding(
        InputAction::HARD_KICK,
        KeyCode::PadTriangle
    );

    setBinding(
        InputAction::BLOCK,
        KeyCode::PadL1
    );

    setBinding(
        InputAction::ENHANCE,
        KeyCode::PadR1
    );
}

// ============================================================
// InputManager Update
// ============================================================

void InputManager::update(
    const std::function<bool(KeyCode)>& isRawKeyDown
)
{
    for (auto& [action, key] : bindings)
    {
        auto& state = actionStates[action];

        bool isDown = isRawKeyDown(key);

        state.wasPressedThisFrame =
            isDown && !state.isHeld;

        state.wasReleasedThisFrame =
            !isDown && state.isHeld;

        state.isHeld = isDown;
    }
}

// ============================================================
// Input Queries
// ============================================================

bool InputManager::isActionHeld(InputAction action) const
{
    auto it = actionStates.find(action);

    return (it != actionStates.end())
        ? it->second.isHeld
        : false;
}

bool InputManager::isActionPressed(InputAction action) const
{
    auto it = actionStates.find(action);

    return (it != actionStates.end())
        ? it->second.wasPressedThisFrame
        : false;
}

bool InputManager::isActionReleased(InputAction action) const
{
    auto it = actionStates.find(action);

    return (it != actionStates.end())
        ? it->second.wasReleasedThisFrame
        : false;
}