#include "GL/glut.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#include "src/Camera.hpp"
#include "src/Renderer.hpp"

Camera camera;

// ============================================================
// Display
// ============================================================

void display()
{
    Renderer::clear(0.08f, 0.08f, 0.12f);

    glLoadIdentity();

    // --------------------------------------------------------
    // Get camera offset
    // --------------------------------------------------------

    float offsetX = camera.getOffsetX();
    float offsetY = camera.getOffsetY();

    // --------------------------------------------------------
    // Draw a square using the camera offset
    // --------------------------------------------------------

    Renderer::drawQuad(
        350.0f + offsetX,
        250.0f + offsetY,
        100.0f,
        100.0f,
        1.0f,
        1.0f,
        1.0f
    );

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

void update(int)
{
    const float deltaTime = 1.0f / 60.0f;

    camera.update(deltaTime);

    // --------------------------------------------------------
    // Print current camera offset
    // --------------------------------------------------------

    printf(
        "Camera Offset -> X: %.2f | Y: %.2f\n",
        camera.getOffsetX(),
        camera.getOffsetY()
    );

    glutPostRedisplay();

    glutTimerFunc(16, update, 0);
}


// ============================================================
// Keyboard
// ============================================================

void keyDown(unsigned char key, int, int)
{
    // --------------------------------------------------------
    // SPACE = strong shake
    // --------------------------------------------------------

    if (key == ' ')
    {
        printf("\nStrong shake triggered!\n");

        camera.shake(
            15.0f,
            0.50f
        );
    }

    // --------------------------------------------------------
    // S = weak shake
    // --------------------------------------------------------

    if (key == 's')
    {
        printf("\nWeak shake triggered!\n");

        camera.shake(
            2.5f,
            0.25f
        );
    }

    // --------------------------------------------------------
    // ESC = exit
    // --------------------------------------------------------

    if (key == 27)
    {
        exit(0);
    }
}


// ============================================================
// Main
// ============================================================

int main(int argc, char** argv)
{
    // ========================================================
    // 1. Seed random number generator
    // ========================================================

    std::srand(static_cast<unsigned int>(std::time(nullptr)));


    // ========================================================
    // 2. Initialize GLUT
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
        "Camera Test"
    );


    // ========================================================
    // 3. Register callbacks
    // ========================================================

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown);


    // ========================================================
    // 4. Start update loop
    // ========================================================

    glutTimerFunc(
        16,
        update,
        0
    );


    // ========================================================
    // 5. Instructions
    // ========================================================

    printf("============================================================\n");
    printf("                       CAMERA TEST\n");
    printf("============================================================\n");

    printf("\nSPACE -> Strong shake (30 intensity, 1 second)\n");
    printf("S     -> Weak shake   (5 intensity, 0.25 seconds)\n");
    printf("ESC   -> Exit\n");

    printf("\nThe white square should shake around its normal position.\n");

    printf("============================================================\n\n");


    // ========================================================
    // 6. Start GLUT
    // ========================================================

    glutMainLoop();

    return 0;
}