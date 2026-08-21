#include "src/Game.hpp"
#include <stdio.h>
#include <algorithm>
#include <cmath>

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

    if (!player1.init(
        "assets/characters/chun-li/idle/", 3,
        "assets/characters/chun-li/jump/", 6
    ))
        printf("Player 1 base animations failed to load!\n");

    if (!player2.init(
        "assets/characters/chun-li/idle/", 3,
        "assets/characters/chun-li/jump-forward/", 5
    ))
        printf("Player 2 base animations failed to load!\n");

    // Optional animations - safe to omit any of these; the matching
    // state falls back to idle if never loaded.
    player1.loadWalkAnimation("assets/characters/chun-li/walk/", 14);
    player1.loadCrouchAnimation("assets/characters/chun-li/crouch/", 3);
    player1.loadBlockAnimation("assets/characters/chun-li/block/", 2);
    player1.loadHitAnimation("assets/characters/chun-li/hit/", 3);

    player2.loadWalkAnimation("assets/characters/chun-li/walk/", 6);
    player2.loadCrouchAnimation("assets/characters/chun-li/crouch/", 3);
    player2.loadBlockAnimation("assets/characters/chun-li/block/", 2);
    player2.loadHitAnimation("assets/characters/chun-li/hit/", 3);

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

    p1Input.applyPlayer1Defaults();
    p2Input.applyPlayer2Defaults();

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

    glViewport(vpX, vpY, vpWidth, vpHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1920.0, 0.0, 1080.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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
void Game::handleJoystick(unsigned int buttonMask, int x, int y, int z)
{
    // Stores the button state from the *last* time this function ran
    static unsigned int previousMask = 0;

    // 1. Find which buttons CHANGED state (XOR)
    unsigned int changed = buttonMask ^ previousMask;

    // 2. Filter out releases: keep only buttons that changed AND are currently down
    unsigned int newlyPressed = changed & buttonMask;

    // 3. Print the index of any newly pressed button
    if (newlyPressed > 0) {
        printf("Button Pressed: ");
        for (int i = 0; i < 32; i++) {
            if (newlyPressed & (1 << i)) {
                printf("[%d] ", i);
            }
        }
        printf("\n");
    }

    // Save the current state to use as history in the next poll cycle
    previousMask = buttonMask;
    glutPostRedisplay();
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

    float dist = p2x - p1x;
    if (std::abs(dist) < MIN_SEPARATION)
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
    // TBD
}

void Game::update()
{
    calculateFPS();

    p1Input.update(isGlutKeyDown);
    p2Input.update(isGlutKeyDown);

    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    // Auto-face the opponent before reading movement input, so
    // FORWARD/BACKWARD stay relative to whoever's closer this frame.
    player1.faceToward(player2.getX());
    player2.faceToward(player1.getX());

    player1.handleInput(p1Input);
    player2.handleInput(p2Input);

    player1.update(deltaTime);
    player2.update(deltaTime);

    resolveStageBounds();
    resolveCombat();
}

void Game::render()
{
    Renderer::clear(0.1f, 0.1f, 0.12f, 1.0f);
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