#include "GL/glut.h"
#include <stdio.h>

#include "src/Character.hpp"
#include "src/Input.hpp"

Character player;
InputManager input;

// ============================================================
// Display
// ============================================================

void display()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    player.render();

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
// Update
// ============================================================

void update(int value)
{
    (void)value;

    // Approximately 60 FPS.
    const float deltaTime = 1.0f / 60.0f;

    input.update([](KeyCode key)
    {
        return isGlutKeyDown(key);
    });

    player.handleInput(input);
    player.update(deltaTime);

    glutPostRedisplay();

    glutTimerFunc(16, update, 0);
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
        GLUT_RGBA |
        GLUT_DOUBLE
    );

    glutInitWindowSize(800, 600);

    glutCreateWindow("Character Test");

    input.applyKeyboardDefaults();

    // ========================================================
    // 2. Test init() BEFORE loading IDLE
    // ========================================================

    printf("============================================================\n");
    printf("                    CHARACTER TEST\n");
    printf("============================================================\n");

    printf("\n=== Test 1: init() before IDLE is loaded ===\n");

    bool initialized = player.init();

    printf(
        "Expected: FAILURE\n"
        "Actual:   %s\n",
        initialized ? "SUCCESS" : "FAILURE"
    );


    // ========================================================
    // 3. Load IDLE
    // ========================================================

    printf("\n=== Test 2: Load IDLE animation ===\n");

    const int idleFrames = 1;

    bool idle = player.loadIdleAnimation(
        "assets/test/",
        idleFrames,
        0.16f
    );

    printf("Frame count: %d\n", idleFrames);
    printf("Loaded: %s\n", idle ? "YES" : "NO");


    if (!idle)
    {
        printf("\nERROR: IDLE animation failed to load.\n");
        return 1;
    }


    // ========================================================
    // 4. Test init() AFTER loading IDLE
    // ========================================================

    printf("\n=== Test 3: init() after IDLE is loaded ===\n");

    initialized = player.init();

    printf(
        "Expected: SUCCESS\n"
        "Actual:   %s\n",
        initialized ? "SUCCESS" : "FAILURE"
    );

    if (!initialized)
    {
        printf("\nERROR: Character initialization failed.\n");
        return 1;
    }


    // ========================================================
    // 5. Load remaining animations
    //
    // Frame counts intentionally differ between 3 and 10.
    // ========================================================

    printf("\n=== Test 4: Load Character Animations ===\n");


    // --------------------------------------------------------
    // WALK
    // --------------------------------------------------------

    int walkFrames = 2;

    bool walk = player.loadWalkAnimation(
        "assets/test/",
        walkFrames,
        0.12f
    );

    printf(
        "WALKING    -> %d frames -> %s\n",
        walkFrames,
        walk ? "LOADED" : "FAILED"
    );


    // --------------------------------------------------------
    // JUMP
    // --------------------------------------------------------

    int jumpFrames = 3;

    bool jump = player.loadJumpAnimation(
        "assets/test/",
        jumpFrames,
        0.10f
    );

    printf(
        "JUMPING    -> %d frames -> %s\n",
        jumpFrames,
        jump ? "LOADED" : "FAILED"
    );


    // --------------------------------------------------------
    // CROUCH
    // --------------------------------------------------------

    int crouchFrames = 4;

    bool crouch = player.loadCrouchAnimation(
        "assets/test/",
        crouchFrames,
        0.14f
    );

    printf(
        "CROUCHING  -> %d frames -> %s\n",
        crouchFrames,
        crouch ? "LOADED" : "FAILED"
    );


    // --------------------------------------------------------
    // BLOCK
    // --------------------------------------------------------

    int blockFrames = 5;

    bool block = player.loadBlockAnimation(
        "assets/test/",
        blockFrames,
        0.11f
    );

    printf(
        "BLOCKING   -> %d frames -> %s\n",
        blockFrames,
        block ? "LOADED" : "FAILED"
    );


    // --------------------------------------------------------
    // HIT STUN
    // --------------------------------------------------------

    int hitStunFrames = 6;

    bool hitStun = player.loadHitStunAnimation(
        "assets/test/",
        hitStunFrames,
        0.08f
    );

    printf(
        "HIT_STUN   -> %d frames -> %s\n",
        hitStunFrames,
        hitStun ? "LOADED" : "FAILED"
    );


    // --------------------------------------------------------
    // KO
    // --------------------------------------------------------

    int koFrames = 7;

    bool ko = player.loadKOAnimation(
        "assets/test/",
        koFrames,
        0.12f
    );

    printf(
        "KO         -> %d frames -> %s\n",
        koFrames,
        ko ? "LOADED" : "FAILED"
    );

    // --------------------------------------------------------
    // Attack
    // --------------------------------------------------------

    int attackFrames = 8;

    bool attack = player.loadAttack(
        AttackType::LIGHT_PUNCH,
        "assets/test/", attackFrames, 0.12f,
        2, 4,
        150,
        15.0f, 35.0f, 65.0f, 65.0f
    );

    printf(
        "Attack     -> %d frames -> %s\n",
        attackFrames,
        attack ? "LOADED" : "FAILED"
    );


    // ========================================================
    // 6. Position / scale test
    // ========================================================

    printf("\n=== Test 5: Character Transform ===\n");

    player.setPosition(400.0f, 100.0f);
    player.setFacing(true);
    player.setScale(1.0f);
    player.setGroundY(50.0f);

    printf("Position: (400, 100)\n");
    printf("Facing: RIGHT\n");
    printf("Scale: 1.0\n");


    // ========================================================
    // 7. Register GLUT callbacks
    // ========================================================

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeyDown);
    glutKeyboardUpFunc(handleKeyUp);


    // ========================================================
    // 8. Start update loop
    // ========================================================

    glutTimerFunc(16, update, 0);


    // ========================================================
    // 9. Instructions
    // ========================================================

    printf("\n============================================================\n");
    printf("                       TEST RUNNING\n");
    printf("============================================================\n");
    printf("\nCharacter should appear around the center of the window.\n");
    printf("The character is being updated at approximately 60 FPS.\n");
    printf("\nClose the window to exit.\n");
    printf("============================================================\n\n");


    // ========================================================
    // 10. Start GLUT event loop
    // ========================================================

    glutMainLoop();

    return 0;
}