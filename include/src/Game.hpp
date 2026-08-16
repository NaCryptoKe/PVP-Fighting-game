#ifndef GAME_HPP
#define GAME_HPP

#include "GL/glut.h"

#include "src/Character.hpp"
#include "src/Font.hpp"
#include "src/Renderer.hpp"

class Game
{
private:
    int windowWidth;
    int windowHeight;
    void calculateFPS();

    Character player1;
    Character player2;

    Font gameFont;

    int lastTime;

public:
    void init();
    Game() : windowWidth(800), windowHeight(600) {} // Initialize with default size
    void updateDimensions(int width, int height);
    void render();
    void update();
    void reshape(int width, int height);
};
#endif // GAME_HPP