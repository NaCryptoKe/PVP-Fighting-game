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
    glClearColor(
        0.08f,
        0.08f,
        0.12f,
        1.0f
    );

    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    player.render();

    glutSwapBuffers();
}


// ============================================================
// Reshape
// ============================================================

void reshape(int width, int height)
{
    if (height == 0)
        height = 1;

    const float targetAspect =
        1920.0f / 1080.0f;

    float windowAspect =
        static_cast<float>(width) /
        static_cast<float>(height);

    int viewportX = 0;
    int viewportY = 0;

    int viewportWidth = width;
    int viewportHeight = height;


    // --------------------------------------------------------
    // Letterboxing / pillarboxing
    // --------------------------------------------------------

    if (windowAspect > targetAspect)
    {
        viewportWidth =
            static_cast<int>(height * targetAspect);

        viewportX =
            (width - viewportWidth) / 2;
    }
    else
    {
        viewportHeight =
            static_cast<int>(width / targetAspect);

        viewportY =
            (height - viewportHeight) / 2;
    }


    glViewport(
        viewportX,
        viewportY,
        viewportWidth,
        viewportHeight
    );


    // --------------------------------------------------------
    // Projection
    // --------------------------------------------------------

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluOrtho2D(
        0.0,
        1920.0,
        0.0,
        1080.0
    );


    // --------------------------------------------------------
    // Model view
    // --------------------------------------------------------

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}


// ============================================================
// Update
// ============================================================

void update(int value)
{
    (void)value;

    const float deltaTime =
        1.0f / 60.0f;


    // --------------------------------------------------------
    // Update input
    // --------------------------------------------------------

    input.update();


    // --------------------------------------------------------
    // Character
    // --------------------------------------------------------

    player.handleInput(input);

    player.update(deltaTime);


    // --------------------------------------------------------
    // Request another frame
    // --------------------------------------------------------

    glutPostRedisplay();

    glutTimerFunc(
        16,
        update,
        0
    );
}


// ============================================================
// Main
// ============================================================

int main(int argc, char** argv)
{
    // ========================================================
    // 1. GLUT initialization
    // ========================================================

    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_RGBA |
        GLUT_DOUBLE
    );

    glutInitWindowSize(
        1280,
        720
    );

    glutCreateWindow(
        "Character Test"
    );


    // ========================================================
    // 2. Input initialization
    // ========================================================

    input.applyKeyboardDefaults();


    // ========================================================
    // 3. Test header
    // ========================================================

    printf(
        "============================================================\n"
    );

    printf(
        "                    CHARACTER TEST\n"
    );

    printf(
        "============================================================\n"
    );


    // ========================================================
    // TEST 1
    // init() before IDLE
    // ========================================================

    printf(
        "\n=== Test 1: init() before IDLE is loaded ===\n"
    );

    bool initialized =
        player.init();

    printf(
        "Expected: FAILURE\n"
        "Actual:   %s\n",
        initialized
            ? "SUCCESS"
            : "FAILURE"
    );


    // ========================================================
    // TEST 2
    // Load IDLE
    // ========================================================

    printf(
        "\n=== Test 2: Load IDLE animation ===\n"
    );

    const int idleFrames = 1;

    bool idle =
        player.loadIdleAnimation(
            "assets/test/",
            idleFrames,
            0.16f
        );

    printf(
        "Frame count: %d\n",
        idleFrames
    );

    printf(
        "Loaded: %s\n",
        idle
            ? "YES"
            : "NO"
    );


    if (!idle)
    {
        printf(
            "\nERROR: IDLE animation failed.\n"
        );

        return 1;
    }


    // ========================================================
    // TEST 3
    // init() after IDLE
    // ========================================================

    printf(
        "\n=== Test 3: init() after IDLE is loaded ===\n"
    );

    initialized =
        player.init();

    printf(
        "Expected: SUCCESS\n"
        "Actual:   %s\n",
        initialized
            ? "SUCCESS"
            : "FAILURE"
    );


    if (!initialized)
    {
        printf(
            "\nERROR: Character initialization failed.\n"
        );

        return 1;
    }


    // ========================================================
    // TEST 4
    // Load all character animations
    // ========================================================

    printf(
        "\n=== Test 4: Load Character Animations ===\n"
    );


    // --------------------------------------------------------
    // WALK
    // --------------------------------------------------------

    int walkFrames = 2;

    bool walk =
        player.loadWalkAnimation(
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

    bool jump =
        player.loadJumpAnimation(
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

    bool crouch =
        player.loadCrouchAnimation(
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

    bool block =
        player.loadBlockAnimation(
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

    bool hitStun =
        player.loadHitStunAnimation(
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

    bool ko =
        player.loadKOAnimation(
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
    // ATTACK
    // --------------------------------------------------------

    int attackFrames = 8;

    bool attack =
        player.loadAttack(
            AttackType::LIGHT_PUNCH,

            "assets/test/",
            attackFrames,
            0.12f,

            2,
            4,

            150,

            15.0f,
            35.0f,
            65.0f,
            65.0f
        );

    printf(
        "LIGHT PUNCH -> %d frames -> %s\n",
        attackFrames,
        attack ? "LOADED" : "FAILED"
    );


    // ========================================================
    // TEST 5
    // Character transform
    // ========================================================

    printf(
        "\n=== Test 5: Character Transform ===\n"
    );


    player.setPosition(
        960.0f,
        100.0f
    );

    player.setGroundY(
        100.0f
    );

    player.setFacing(
        true
    );

    player.setScale(
        1.0f
    );


    printf(
        "Position: (960, 100)\n"
    );

    printf(
        "Ground:   100\n"
    );

    printf(
        "Facing:   RIGHT\n"
    );

    printf(
        "Scale:    1.0\n"
    );


    // ========================================================
    // TEST 6
    // Hurtbox setup
    // ========================================================

    printf(
        "\n=== Test 6: Hurtbox Setup ===\n"
    );

    player.setHurtboxes(
        80.0f,    // standing width
        180.0f,   // standing height
        90.0f,    // crouching width
        110.0f    // crouching height
    );

    printf(
        "Standing hurtbox: 80 x 180\n"
    );

    printf(
        "Crouching hurtbox: 90 x 110\n"
    );


    // ========================================================
    // TEST 7
    // Combat configuration
    // ========================================================

    printf(
        "\n=== Test 7: Combat Configuration ===\n"
    );

    printf(
        "Health: %d / %d\n",
        player.getHealth(),
        player.getMaxHealth()
    );

    printf(
        "KO: %s\n",
        player.isKO()
            ? "YES"
            : "NO"
    );

    printf(
        "Facing right: %s\n",
        player.getFacingRight()
            ? "YES"
            : "NO"
    );


    // ========================================================
    // GLUT callbacks
    // ========================================================

    glutDisplayFunc(
        display
    );

    glutReshapeFunc(
        reshape
    );

    glutKeyboardFunc(
        handleKeyDown
    );

    glutKeyboardUpFunc(
        handleKeyUp
    );


    // ========================================================
    // Start update loop
    // ========================================================

    glutTimerFunc(
        16,
        update,
        0
    );


    // ========================================================
    // Controls
    // ========================================================

    printf(
        "\n============================================================\n"
    );

    printf(
        "                       TEST RUNNING\n"
    );

    printf(
        "============================================================\n"
    );

    printf(
        "\nControls:\n"
    );

    printf(
        "W       -> Jump\n"
    );

    printf(
        "S       -> Crouch\n"
    );

    printf(
        "A       -> Move backward\n"
    );

    printf(
        "D       -> Move forward\n"
    );

    printf(
        "Space   -> Block\n"
    );

    printf(
        "\n"
    );

    printf(
        "Character starts at the center of the world.\n"
    );

    printf(
        "Virtual resolution: 1920 x 1080\n"
    );

    printf(
        "Update rate: approximately 60 FPS\n"
    );

    printf(
        "\n============================================================\n"
    );


    // ========================================================
    // GLUT event loop
    // ========================================================

    glutMainLoop();

    return 0;
}