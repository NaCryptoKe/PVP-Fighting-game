#ifndef GAME_HPP
#define GAME_HPP

#include "GL/glut.h"

class Game
{
private:
    int windowWidth;
    int windowHeight;
    void renderBitmapString(float x, float y, void *font, const char *string);
    void calculateFPS();

    GLuint player1;
    GLuint player2;

public:
    void init();
    Game() : windowWidth(800), windowHeight(600) {} // Initialize with default size
    void updateDimensions(int width, int height);
    void render();
    void update();
    void reshape(int width, int height);
};
#endif // GAME_HPP