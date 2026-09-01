#include "GL/glut.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <algorithm>

#include "src/Camera.hpp"
#include "src/Renderer.hpp"

Camera camera;

// Virtual Stage & Player state
const float STAGE_LEFT  = 0.0f;
const float STAGE_RIGHT = 1600.0f;

float p1X = 600.0f;
float p2X = 1000.0f;

bool keys[256] = { false };
bool specialKeys[256] = { false };

// ============================================================
// Display
// ============================================================

void display()
{
    Renderer::clear(0.08f, 0.08f, 0.12f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float zoom = camera.getZoom();
    float camX = camera.getCenterX() + camera.getOffsetX();
    
    float visibleWidth  = 800.0f / zoom;
    float visibleHeight = 600.0f / zoom;

    // Horizontal bounds centered around camX
    float leftX  = camX - (visibleWidth * 0.5f);
    float rightX = camX + (visibleWidth * 0.5f);

    // Bottom anchored at 0.0f + shake offset so ground never floats
    float bottomY = 0.0f + camera.getOffsetY();
    float topY    = visibleHeight + camera.getOffsetY();

    glOrtho(leftX, rightX, bottomY, topY, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Floor (at Y = 50.0f)
    Renderer::drawQuad(STAGE_LEFT, 50.0f, STAGE_RIGHT - STAGE_LEFT, 10.0f, 0.3f, 0.3f, 0.3f);
    // Boundary Walls
    Renderer::drawQuad(STAGE_LEFT, 50.0f, 10.0f, 500.0f, 0.8f, 0.2f, 0.2f);
    Renderer::drawQuad(STAGE_RIGHT - 10.0f, 50.0f, 10.0f, 500.0f, 0.8f, 0.2f, 0.2f);

    // Players
    Renderer::drawQuad(p1X - 25.0f, 60.0f, 50.0f, 100.0f, 0.9f, 0.3f, 0.3f);
    Renderer::drawQuad(p2X - 25.0f, 60.0f, 50.0f, 100.0f, 0.3f, 0.5f, 0.9f);

    glutSwapBuffers();
}

// ============================================================
// Reshape
// ============================================================

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    camera.setViewportSize(800.0f, 600.0f);
}

// ============================================================
// Update
// ============================================================

void update(int)
{
    const float deltaTime = 1.0f / 60.0f;
    const float moveSpeed = 400.0f * deltaTime;

    // Player 1 Movement (A / D)
    if (keys['a'] || keys['A']) p1X = std::max(STAGE_LEFT + 25.0f, p1X - moveSpeed);
    if (keys['d'] || keys['D']) p1X = std::min(STAGE_RIGHT - 25.0f, p1X + moveSpeed);

    // Player 2 Movement (Left / Right Arrow)
    if (specialKeys[GLUT_KEY_LEFT])  p2X = std::max(STAGE_LEFT + 25.0f, p2X - moveSpeed);
    if (specialKeys[GLUT_KEY_RIGHT]) p2X = std::min(STAGE_RIGHT - 25.0f, p2X + moveSpeed);

    // Update Camera Framing & Shake Decay
    camera.follow(p1X, p2X, deltaTime);
    camera.update(deltaTime);

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// ============================================================
// Keyboard Handlers
// ============================================================

void keyDown(unsigned char key, int, int)
{
    keys[key] = true;

    if (key == ' ') camera.shake(15.0f, 0.50f); // Strong shake
    if (key == 's' || key == 'S') camera.shake(4.0f, 0.25f); // Weak shake
    if (key == 27) exit(0); // ESC
}

void keyUp(unsigned char key, int, int)
{
    keys[key] = false;
}

void specialKeyDown(int key, int, int)
{
    if (key < 256) specialKeys[key] = true;
}

void specialKeyUp(int key, int, int)
{
    if (key < 256) specialKeys[key] = false;
}

// ============================================================
// Main
// ============================================================

int main(int argc, char** argv)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Camera System Test");

    // Camera Initial Setup
    camera.setViewportSize(800.0f, 600.0f);
    camera.setStageBounds(STAGE_LEFT, STAGE_RIGHT);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);

    glutTimerFunc(16, update, 0);

    printf("============================================================\n");
    printf("                  INTERACTIVE CAMERA TEST                   \n");
    printf("============================================================\n");
    printf("  A / D        -> Move Player 1 (Red)\n");
    printf("  LEFT / RIGHT -> Move Player 2 (Blue)\n");
    printf("  SPACE        -> Strong Screen Shake\n");
    printf("  S            -> Light Screen Shake\n");
    printf("  ESC          -> Exit\n");
    printf("============================================================\n\n");

    glutMainLoop();
    return 0;
}

// run with: g++ -std=c++17 -Wall -Wextra -Iinclude -lGL -lGLU -lglut src/Camera.cpp test/camera_test.cpp -o camera_test && ./camera_test