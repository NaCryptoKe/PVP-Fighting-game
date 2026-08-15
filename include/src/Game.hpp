#ifndef GAME_HPP
#define GAME_HPP

#include "GL/glut.h"

#include "src/Sprite.hpp"
#include "src/Animation.hpp"
#include "src/Font.hpp"

class Game
{
private:
    int windowWidth;
    int windowHeight;
    void calculateFPS();

    Sprite player1;
    Sprite player2;

    Animation player1Anim;
    Animation player2Anim;

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