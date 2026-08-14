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
    if(height == 0) height = 1;
    glViewport(0,0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Bottom left corner is (0,0)
    gluOrtho2D(0.0, width, 0.0, height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Let the game class know the resolution
    updateDimensions(width, height);
}

void Game::update()
{
    calculateFPS();
}