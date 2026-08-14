#include "src/Game.hpp"
#include "src/Renderer.hpp"
#include "src/Texture.hpp"
#include <stdio.h>

char fpsString[32] = "FPS: 0.0";
int frameCount = 0;
int previousTime = 0;

GLuint createCheckerboardTexture() {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // 2x2 grid of RGBA pixels (Magenta and Black)
    GLubyte pixels[] = {
        255, 0, 255, 255,    0,   0,   0, 255,  // Row 1: Magenta, Black
          0, 0,   0, 255,  255,   0, 255, 255   // Row 2: Black, Magenta
    };

    // Set filtering to GL_NEAREST so pixels stay crisp (perfect for pixel art)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // Texture wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Upload pixel data to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    return textureID;
}

GLuint createSolidWhiteTexture() {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLubyte pixel[] = { 255, 255, 255, 255 }; // Single White Pixel

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

    return textureID;
}

void Game::init()
{
    player1 = loadTexture("assets/sprites/player.png");
    player2 = loadTexture("assets/sprites/player.png");
}

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
    // Clear Screen
    Renderer::clear(0.1f, 0.1f, 0.12f, 1.0f);
    glLoadIdentity();

    // Render screen-space FPS counter
    glColor3f(1.0f, 1.0f, 1.0f);
    renderBitmapString(20.0f, windowHeight - 50.0f, GLUT_BITMAP_TIMES_ROMAN_24, fpsString);

    float floor = 150.0f;
    // Floor
    Renderer::drawQuad(0.0f, 0.0f, 1920.0f, floor, 0.3f, 0.3f, 0.3f, 0.35f);

    float p1X = 400.0f;
    //float p1Y = 150.0f;
    float p2X = windowWidth - 620.0f;
    //float p2Y = 150.0f;
    // Player 1 (Facing Right)
    Renderer::drawFighterSprite(player1, p1X, floor, 180.0f, 270.0f, 1.2f, false);

    // Player 2 (Facing Left)
    Renderer::drawFighterSprite(player2, p2X, floor, 180.0f, 270.0f, 0.75f, false);

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