#include "src/Game.hpp"
#include <stdio.h>

char fpsString[32] = "FPS: 0.0";
int frameCount = 0;
int previousTime = 0;

void Game::updateDimensions(int width, int height)
{
    windowWidth     = width;
    windowHeight    = height;
}

void Game::renderBitmapString(float x, float y, void *font, const char *string)
{
    glRasterPos2f(x, y);
    for (const char *c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void Game::calculateFPS()
{
    frameCount++;

    int currentTime = glutGet(GLUT_ELAPSED_TIME); // Total time in milliseconds
    int timeInterval = currentTime - previousTime; // Change in time

    if (timeInterval > 1000) // Exceeds 1 second
    {
        float fps = frameCount / (timeInterval / 1000.0f);
        previousTime = currentTime;
        frameCount = 0;

        snprintf(fpsString, sizeof(fpsString), "FPS: %.1f", fps);
    }
}

void Game::render()
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // Render screen-space FPS counter
    glColor3f(1.0f, 1.0f, 1.0f);
    renderBitmapString(20.0f, windowHeight - 50.0f, GLUT_BITMAP_TIMES_ROMAN_24, fpsString);

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2i(100, 100); // Bottom-left
        glVertex2i(300, 100); // Bottom-right
        glVertex2i(300, 300); // Top-right
        glVertex2i(100, 300); // Top-left
    glEnd();

    glutSwapBuffers();
}

void Game::reshape(int width, int height) {
    if (height == 0) height = 1;

    // Target aspect ratio: 1920 / 1080 = 1.7777...
    const float targetAspect = 1920.0f / 1080.0f;
    float windowAspect = (float)width / (float)height;

    int vpX = 0, vpY = 0;
    int vpWidth = width, vpHeight = height;

    if (windowAspect > targetAspect) {
        // Window is wider than 16:9 -> Pillarbox (black bars on left/right)
        vpWidth = static_cast<int>(height * targetAspect);
        vpX = (width - vpWidth) / 2;
    } else {
        // Window is taller than 16:9 -> Letterbox (black bars on top/bottom)
        vpHeight = static_cast<int>(width / targetAspect);
        vpY = (height - vpHeight) / 2;
    }

    // 1. Set centered viewport inside the window
    glViewport(vpX, vpY, vpWidth, vpHeight);

    // 2. Lock world coordinate system permanently to 1920x1080
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1920.0, 0.0, 1080.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 3. Always pass virtual dimensions (1920x1080) to game logic
    updateDimensions(1920, 1080);
}

void Game::update()
{
    calculateFPS();
}