#include "src/Game.hpp"
#include <stdio.h>
#include <algorithm>
#include <cmath>

void Game::enterState(GameState state)
{
    switch (state)
    {
        case GameState::MAIN_MENU:
            // - Start playing main menu BGM loop
            // - Reset menu cursor/selection back to top item ("Fight")
            break;

        case GameState::CHARACTER_SELECT:
            // - Load character roster portraits and model previews
            // - Reset P1 and P2 selection state to "unconfirmed"
            break;

        case GameState::FIGHTING:
            // - Reset RoundTimer (e.g., roundTimer.reset(90))
            // - Spawn P1 and P2 at starting stage positions with 100% health
            // - Clear existing projectile/hitbox entities
            // - Switch BGM to chosen stage music track
            break;

        case GameState::PAUSED:
            // - Pause audio channels/effects (except pause menu UI sounds)
            // - Set UI focus to "Resume" menu option
            // - Capture screen blur or dark overlay snapshot
            break;

        case GameState::ROUND_OVER:
            // - Play "K.O." or "Time Up" announcer sound effect
            // - Disable character movement and attack inputs
            // - Increment round win counter for the winning player
            // - Trigger hit-freeze or slow-motion camera effect
            break;

        case GameState::MATCH_OVER:
            // - Play victory screen BGM and winner announcer line
            // - Display final match scores and rematch UI overlay
            // - Unlock any achievements earned during the match
            break;

        case GameState::SETTINGS:
            // - Load current settings from Config object into UI controls
            // - Highlight default setting field (e.g., Master Volume)
            break;
    }
}

void Game::exitState(GameState state)
{
    switch (state)
    {
        case GameState::MAIN_MENU:
            // - Fade out or stop main menu BGM
            break;

        case GameState::CHARACTER_SELECT:
            // - Free pre-rendered portrait textures/model previews from VRAM
            break;

        case GameState::FIGHTING:
            // - Stop stage combat music
            // - Clean up remaining particle effects or dynamic debris
            break;

        case GameState::PAUSED:
            // - Resume audio channels/effects
            // - Remove dark overlay/blur filter
            break;

        case GameState::ROUND_OVER:
            // - Reset screen flash / camera shake effects
            break;

        case GameState::MATCH_OVER:
            // - Reset match win counters for both players back to 0
            break;

        case GameState::SETTINGS:
            // - Save changes to "config.ini" on disk (e.g., config.save("config.ini"))
            break;
    }
}

void Game::requestStateChange(GameState newState)
{
    pendingState = newState;
    stateChangeRequested = true;
}

void Game::processStateChange()
{
    if (!stateChangeRequested || currentState == pendingState)
        return;

    exitState(currentState);
    currentState = pendingState;
    enterState(currentState);

    stateChangeRequested = false;
}

char fps[] = "FPS: ";
char fpsString[32] = "0.0";
int frameCount = 0;
int previousTime = 0;

static const float FLOOR_Y = 150.0f;
static const float STAGE_LEFT = 60.0f;
static const float STAGE_RIGHT = 1920.0f - 60.0f;
static const float MIN_SEPARATION = 90.0f; // minimum horizontal gap kept between players

void Game::init()
{
    if (!gameFont.load("assets/fonts/mainFont.ttf", 32.0f)) printf("Font loading failed!\n");

    if (!player1.init())
        printf("Player 1 failed to initialize!\n");

    if (!player2.init())
        printf("Player 2 failed to initialize!\n");

    // Idle/jump are the only animations Character strictly requires
    // to render/update; everything below is optional and falls back
    // to idle if never loaded. Durations match the per-frame timings
    // used in test/character_test.cpp.
    if (!player1.loadIdleAnimation("assets/characters/chun-li/idle/", 3, 0.16f))
        printf("Player 1 idle animation failed to load!\n");
    if (!player1.loadJumpAnimation("assets/characters/chun-li/jump/", 6, 0.10f))
        printf("Player 1 jump animation failed to load!\n");

    if (!player2.loadIdleAnimation("assets/characters/chun-li/idle/", 3, 0.16f))
        printf("Player 2 idle animation failed to load!\n");
    if (!player2.loadJumpAnimation("assets/characters/chun-li/jump-forward/", 5, 0.10f))
        printf("Player 2 jump animation failed to load!\n");

    player1.loadWalkAnimation("assets/characters/chun-li/walk/", 14, 0.12f);
    player1.loadCrouchAnimation("assets/characters/chun-li/crouch/", 3, 0.14f);
    player1.loadBlockAnimation("assets/characters/chun-li/block/", 2, 0.11f);
    player1.loadHitStunAnimation("assets/characters/chun-li/hit/", 3, 0.08f);

    player2.loadWalkAnimation("assets/characters/chun-li/walk/", 6, 0.12f);
    player2.loadCrouchAnimation("assets/characters/chun-li/crouch/", 3, 0.14f);
    player2.loadBlockAnimation("assets/characters/chun-li/block/", 2, 0.11f);
    player2.loadHitStunAnimation("assets/characters/chun-li/hit/", 3, 0.08f);

    // Attacks - optional too. Active frames / damage / hitbox size are
    // placeholders; tune once real frame data and hitbox art exist.
    // hbOffsetX/Y/width/height are authored assuming the character
    // faces RIGHT (mirrored automatically for a character facing left).
    player1.loadAttack(AttackType::LIGHT_PUNCH,
        "assets/characters/chun-li/punch-light/", 4, 0.06f,
        1, 2, 6,
        40.0f, 80.0f, 60.0f, 30.0f);

    player1.loadAttack(AttackType::LIGHT_KICK,
        "assets/characters/chun-li/kick-light/", 5, 0.07f,
        2, 3, 8,
        40.0f, 40.0f, 70.0f, 30.0f);

    player2.loadAttack(AttackType::LIGHT_PUNCH,
        "assets/characters/chun-li/punch-light/", 4, 0.06f,
        1, 2, 6,
        40.0f, 80.0f, 60.0f, 30.0f);

    player2.loadAttack(AttackType::LIGHT_KICK,
        "assets/characters/chun-li/kick-light/", 5, 0.07f,
        2, 3, 8,
        40.0f, 40.0f, 70.0f, 30.0f);

    player1.setFacing(true);
    player2.setFacing(false);

    player1.setScale(3);
    player2.setScale(3);

    player1.setGroundY(FLOOR_Y);
    player2.setGroundY(FLOOR_Y);
    player1.setPosition(600.0f, FLOOR_Y);
    player2.setPosition(1320.0f, FLOOR_Y);

    // Config.load() falls back to (and returns false for) built-in
    // defaults if config.ini doesn't exist yet or fails to parse -
    // save() then writes it out so it exists for next launch.
    if (!config.load(CONFIG_PATH))
    {
        config.save(CONFIG_PATH);
    }

    config.applyBindings(p1Input, true);
    config.applyBindings(p2Input, false);

    player1.setVolume(config.getMasterVolume());
    player2.setVolume(config.getMasterVolume());

    // Player 2's default bindings are gamepad keys (see Config's
    // built-in defaults) - without a connected controller those
    // bindings simply never fire, same as an unplugged keyboard.
    if (!initGamepad())
        printf("No gamepad connected - Player 2 pad input unavailable until one connects.\n");

    lastTime = glutGet(GLUT_ELAPSED_TIME);
}

void Game::updateDimensions(int width, int height)
{
    windowWidth  = width;
    windowHeight = height;
}

void Game::calculateFPS()
{
    frameCount++;

    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int timeInterval = currentTime - previousTime;

    if (timeInterval > 1000)
    {
        float fps = frameCount / (timeInterval / 1000.0f);
        previousTime = currentTime;
        frameCount = 0;

        snprintf(fpsString, sizeof(fpsString), "%.1f", fps);
    }
}

void Game::reshape(int width, int height) {
    if (height == 0) height = 1;

    const float targetAspect = 1920.0f / 1080.0f;
    float windowAspect = (float)width / (float)height;

    int vpX = 0, vpY = 0;
    int vpWidth = width, vpHeight = height;

    if (windowAspect > targetAspect) {
        vpWidth = static_cast<int>(height * targetAspect);
        vpX = (width - vpWidth) / 2;
    } else {
        vpHeight = static_cast<int>(width / targetAspect);
        vpY = (height - vpHeight) / 2;
    }

    // Maps the OpenGL rendering area to the letterboxed window space
    glViewport(vpX, vpY, vpWidth, vpHeight);

    // REMOVED: glMatrixMode(GL_PROJECTION) and gluOrtho2D
    // The dynamic Camera now handles projection every frame.

    updateDimensions(1920, 1080);
}

/*
 * Joystick Layout of the PS4 Joystick
 * 0 - Cross
 * 1 - Circle
 * 2 - Triangle
 * 3 - Square
 * 4 - L1
 * 5 - R1
 * 6 - L2
 * 7 - R2
 * 11 - L3
 * 12 - R3
 * 10 - PS button
 * 9 - Option
 * 8 - Share
*/
namespace
{
    // Every KeyCode the gamepad can report - polled once per frame
    // in Game::pollGamepad(). Kept as an explicit list rather than
    // iterating 0..COUNT so the keyboard KeyCodes (W/S/D/A/etc.)
    // never get swept in here by accident.
    constexpr KeyCode PAD_KEYS[] = {
        KeyCode::PadCross, KeyCode::PadCircle, KeyCode::PadSquare, KeyCode::PadTriangle,
        KeyCode::PadL1, KeyCode::PadR1, KeyCode::PadL2, KeyCode::PadR2,
        KeyCode::PadL3, KeyCode::PadR3,
        KeyCode::PadStickUp, KeyCode::PadStickDown, KeyCode::PadStickLeft, KeyCode::PadStickRight,
        KeyCode::PadDPADUp, KeyCode::PadDPADDown, KeyCode::PadDPADLeft, KeyCode::PadDPADRight,
    };
}

void Game::pollGamepad()
{
    updateGamepad();

    // isGamepadKeyDown() is state, not events - InputManager::update()
    // only reacts to pushInputEvent() transitions (the same queue
    // GLUT's keyboard callbacks feed). So we diff against last
    // frame's state here and push a transition event exactly once,
    // the same way a real key-down/key-up callback would.
    static bool previousPadState[static_cast<int>(KeyCode::COUNT)] = { false };

    for (KeyCode key : PAD_KEYS)
    {
        bool isDown = isGamepadKeyDown(key);
        int index = static_cast<int>(key);

        if (isDown != previousPadState[index])
        {
            pushInputEvent(key, isDown);
            previousPadState[index] = isDown;
        }
    }
}

void Game::resolveCombat()
{
    struct Pair { Character* attacker; Character* defender; };
    Pair pairs[2] = { { &player1, &player2 }, { &player2, &player1 } };

    for (auto& pair : pairs)
    {
        if (!pair.attacker->hasActiveHitbox())
            continue;

        AABB atkBox = pair.attacker->getActiveHitboxWorld();
        AABB defBox = pair.defender->getHurtboxWorld();

        if (aabbOverlap(atkBox, defBox))
        {
            bool blocked = pair.defender->isBlocking();
            pair.defender->applyHit(pair.attacker->getActiveHitboxDamage(), blocked);
            pair.attacker->registerHitLanded();
        }
    }
}

void Game::resolveStageBounds()
{
    float p1x = player1.getX();
    float p2x = player2.getX();

    p1x = std::max(STAGE_LEFT, std::min(STAGE_RIGHT, p1x));
    p2x = std::max(STAGE_LEFT, std::min(STAGE_RIGHT, p2x));

    // Only push players apart if they could plausibly be colliding -
    // i.e. their hurtboxes actually overlap vertically too, not just
    // horizontally. Without this check, a player jumping over their
    // opponent's head (same X, very different Y) was still treated
    // as a horizontal collision and shoved sideways instead of being
    // allowed to pass overhead.
    AABB p1Box = player1.getHurtboxWorld();
    AABB p2Box = player2.getHurtboxWorld();
    bool verticallyOverlapping = (p1Box.bottom < p2Box.top) && (p2Box.bottom < p1Box.top);

    float dist = p2x - p1x;
    if (verticallyOverlapping && std::abs(dist) < MIN_SEPARATION)
    {
        float overlap = MIN_SEPARATION - std::abs(dist);
        float push = overlap / 2.0f;

        if (dist >= 0.0f) { p1x -= push; p2x += push; }
        else               { p1x += push; p2x -= push; }

        p1x = std::max(STAGE_LEFT, std::min(STAGE_RIGHT, p1x));
        p2x = std::max(STAGE_LEFT, std::min(STAGE_RIGHT, p2x));
    }

    player1.setPosition(p1x, player1.getY());
    player2.setPosition(p2x, player2.getY());
}

void Game::drawDebugBoxes(Character &c)
{
    AABB hurt = c.getHurtboxWorld();
    Renderer::drawQuad(
        hurt.left, hurt.bottom,
        hurt.right - hurt.left, hurt.top - hurt.bottom,
        0.0f, 1.0f, 0.4f, 0.8f
    );

    // Implement hitbox while active next
    if (c.hasActiveHitbox())
    {
        AABB atk = c.getActiveHitboxWorld();
        Renderer::drawQuad(
            atk.left, atk.bottom,
            atk.right - atk.left, atk.top - atk.bottom,
            1.0f, 0.0f, 0.0f, 0.04f
        );
    }
}

void Game::update()
{
    // ============================================================
    // 1. Global Per-Frame Housekeeping & Input Polling
    // ============================================================
    calculateFPS();
    pollGamepad();

    // Process queued state changes from previous frame (e.g. Pause pressed, KO triggered)
    processStateChange();

    // Update input state machines & clear shared queue
    p1Input.update();
    p2Input.update();
    clearInputEvents();

    // Compute Delta Time
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    // ============================================================
    // 2. State-Specific Gameplay Logic
    // ============================================================
    switch (currentState)
    {
        case GameState::FIGHTING:
        {
            // Advance round timer
            roundTimer.update(deltaTime);

            // Check if time expired
            if (roundTimer.isExpired())
            {
                requestStateChange(GameState::ROUND_OVER);
                break;
            }

            // Facing direction (cosmetics & hitbox orientation)
            player1.faceToward(player2.getX());
            player2.faceToward(player1.getX());

            // Process combat movement & inputs
            player1.handleInput(p1Input);
            player2.handleInput(p2Input);

            // Physics & animation step
            player1.update(deltaTime);
            player2.update(deltaTime);

            // Collisions & health checks
            resolveStageBounds();
            resolveCombat();

            // Check if a player lost all health
            if (player1.getHealth() <= 0 || player2.getHealth() <= 0)
            {
                requestStateChange(GameState::ROUND_OVER);
                break;
            }

            // Camera tracking
            camera.follow(player1.getX(), player2.getX(), deltaTime);
            camera.update(deltaTime);
            break;
        }

        case GameState::PAUSED:
            // Input managers are already updated above to detect "Unpause" button presses.
            // Player movement, physics, RoundTimer, and combat resolution are deliberately skipped.
            break;

        case GameState::ROUND_OVER:
            // Let the camera continue smoothly tracking or run round-end camera zoom
            camera.update(deltaTime);
            
            // Advance KO/Round-over animation timer; once finished, transition to FIGHTING or MATCH_OVER
            break;

        case GameState::MAIN_MENU:
        case GameState::CHARACTER_SELECT:
        case GameState::SETTINGS:
        case GameState::MATCH_OVER:
            // Handle menu specific updates or UI navigation here
            break;
    }
}
void Game::render()
{
    Renderer::clear(0.1f, 0.1f, 0.12f, 1.0f);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float camX = camera.getCenterX() + camera.getOffsetX();
    float camY = (1080.0f * 0.5f) + camera.getOffsetY(); // Fixed vertical center

    float zoom = camera.getZoom();
    float halfWidth = (1920.0f / zoom ) * 0.5f;
    float halfHeight = (1080.0f / zoom ) * 0.5f;
    
    gluOrtho2D(
        camX - halfWidth,
        camX + halfHeight,
        camY - halfHeight,
        camY + halfHeight
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gameFont.renderText(fpsString, 180.0f, 1030.0f, 1.0f, 1.0f, 0.0f);
    gameFont.renderText(fps, 20.0f, 1030.0f, 1.0f, 0.2f, 0.0f);

    Renderer::drawQuad(0.0f, 0.0f, 1920.0f, FLOOR_Y, 0.3f, 0.3f, 0.3f, 0.35f);

    player1.render();
    player2.render();

    drawDebugBoxes(player1);
    drawDebugBoxes(player2);

    glutSwapBuffers();
}