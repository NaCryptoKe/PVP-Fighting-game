#include "GL/glut.h"
#include <stdio.h>

#include "src/RoundTimer.hpp"
#include "src/HUD.hpp"
#include "src/Font.hpp"
#include "src/Renderer.hpp"

// ============================================================
// Global objects
// ============================================================

RoundTimer roundTimer;
Font font;

Character player;

HUD hud(player, 450.0f);

// ============================================================
// Display
// ============================================================

void display()
{
    Renderer::clear(0.08f, 0.08f, 0.12f);

    glLoadIdentity();

    // --------------------------------------------------------
    // Get remaining seconds from RoundTimer
    // and pass them directly to the HUD.
    // --------------------------------------------------------

    hud.drawTimer(
        font,
        roundTimer.getSecondsRemaining(),
        100.0f
    );

    glutSwapBuffers();
}


// ============================================================
// Reshape
// ============================================================

void reshape(int width, int height) {
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

    hud.setViewportSize(
        static_cast<float>(width),
        static_cast<float>(height)
    );

    glViewport(vpX, vpY, vpWidth, vpHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1920.0, 0.0, 1080.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ============================================================
// Update
// ============================================================

void update(int)
{
    // Approximately 60 FPS
    const float deltaTime = 1.0f / 60.0f;

    // --------------------------------------------------------
    // Update the RoundTimer
    // --------------------------------------------------------

    roundTimer.update(deltaTime);

    // --------------------------------------------------------
    // Print timer occasionally so we can verify it
    // --------------------------------------------------------

    static int previousSecond = -1;

    int currentSecond =
        roundTimer.getSecondsRemaining();

    if (currentSecond != previousSecond)
    {
        printf(
            "Timer: %d seconds | Expired: %s\n",
            currentSecond,
            roundTimer.isExpired() ? "YES" : "NO"
        );

        previousSecond = currentSecond;
    }

    // --------------------------------------------------------
    // Redraw
    // --------------------------------------------------------

    glutPostRedisplay();

    // --------------------------------------------------------
    // Schedule next update
    // --------------------------------------------------------

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
        GLUT_DOUBLE |
        GLUT_RGBA
    );

    glutInitWindowSize(800, 600);

    glutCreateWindow("Round Timer Test");


    // ========================================================
    // 2. Load font
    // ========================================================

    printf("Loading font...\n");

    if (!font.load(
        "assets/fonts/mainFont.ttf",
        48.0f
    ))
    {
        printf("ERROR: Failed to load font.\n");
        return 1;
    }

    printf("Font loaded successfully.\n");


    // ========================================================
    // 3. Create HUD
    // ========================================================

    // hud = new HUD(
    //     player,
    //     450.0f
    // );


    // ========================================================
    // 4. Initialize RoundTimer
    // ========================================================

    printf("\n");
    printf("============================================================\n");
    printf("                    ROUND TIMER TEST\n");
    printf("============================================================\n");

    roundTimer.reset(10);

    printf(
        "Starting timer: %d seconds\n",
        roundTimer.getSecondsRemaining()
    );


    // ========================================================
    // 5. Register GLUT callbacks
    // ========================================================

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);


    // ========================================================
    // 6. Start update loop
    // ========================================================

    glutTimerFunc(
        16,
        update,
        0
    );


    // ========================================================
    // 7. Start GLUT
    // ========================================================

    glutMainLoop();

    // delete hud;

    return 0;
}