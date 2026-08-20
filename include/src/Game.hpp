#ifndef GAME_HPP
#define GAME_HPP

#include "GL/glut.h"

#include "src/Character.hpp"
#include "src/Font.hpp"
#include "src/Renderer.hpp"
#include "src/Input.hpp"

class Game
{
private:
    int windowWidth;
    int windowHeight;
    void calculateFPS();

    Character player1;
    Character player2;

    InputManager p1Input;
    InputManager p2Input;

    Font gameFont;

    int lastTime;

    void resolveCombat();       // hit detection between player1/player2
    void resolveStageBounds();  // keep both players on-screen and apart

    void drawDebugBoxes(Character &c);

public:
    void init();
    Game() : windowWidth(800), windowHeight(600) {}
    void updateDimensions(int width, int height);
    void render();
    void update();
    void reshape(int width, int height);
};
#endif // GAME_HPP