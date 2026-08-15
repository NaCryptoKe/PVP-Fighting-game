#include "src/Game.hpp"
#include "src/Renderer.hpp"
#include "src/Texture.hpp"
#include "src/Font.hpp"
#include "src/Sprite.hpp"
#include <stdio.h>

char fps[] = "FPS: ";
char fpsString[32] = "0.0";
int frameCount = 0;
int previousTime = 0;

Font gameFont;
Sprite player1;
Sprite player2;
void Game::init()
{
    if (!gameFont.load("assets/fonts/street-fighter-alpha-xl-colour.colr.ttf", 32.0f)) {
        printf("Font loading failed!\n");
    }
    GLuint tex1 = loadTexture("assets/characters/ryu/jump-forward/00.png");
    GLuint tex2 = loadTexture("assets/characters/player.png");
    player1 = Sprite(tex1, 180.0f, 270.0f);
    player2 = Sprite(tex2, 180.0f, 270.0f);
}

void Game::updateDimensions(int width, int height)
{
    windowWidth     = width;
    windowHeight    = height;
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

        snprintf(fpsString, sizeof(fpsString), "%.1f", fps);
    }
}

void Game::render()
{
    // Clear Screen
    Renderer::clear(0.1f, 0.1f, 0.12f, 1.0f);
    glLoadIdentity();

    // Render screen-space FPS counter
    gameFont.renderText(fpsString, 180.0f, 1030.0f, 1.0f, 1.0f, 0.0f);
    gameFont.renderText(fps, 20.0f, 1030.0f, 1.0f, 0.2f, 0.0f);

    float floor = 150.0f;
    // Floor
    Renderer::drawQuad(0.0f, 0.0f, 1920.0f, floor, 0.3f, 0.3f, 0.3f, 0.35f);

    float p1X = 400.0f;
    //float p1Y = 150.0f;
    float p2X = windowWidth - 620.0f;
    //float p2Y = 150.0f;
    // Player 1 (Facing Right)
    player1.setFlip(true);
    player1.setPosition(p1X, floor);
    player1.setScale(1.2f);
    player1.draw();

    // Player 2 (Facing Left)
    player2.setFlip(false);
    player2.setPosition(p2X, floor);
    player2.setScale(1.0f);
    player2.draw();

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