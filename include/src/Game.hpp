#ifndef GAME_HPP
#define GAME_HPP

#include "GL/glut.h"

#include "src/Character.hpp"
#include "src/Font.hpp"
#include "src/Renderer.hpp"
#include "src/Input.hpp"
#include "src/Config.hpp"
#include "src/Camera.hpp"

class Game
{
private:
    int windowWidth;
    int windowHeight;
    void calculateFPS();

    Camera camera;

    Character player1;
    Character player2;

    InputManager p1Input;
    InputManager p2Input;

    Config config;
    static constexpr const char* CONFIG_PATH = "config.ini";

    Font gameFont;

    int lastTime;

    void resolveCombat();       // hit detection between player1/player2
    void resolveStageBounds();  // keep both players on-screen and apart
    void pollGamepad();         // bridges SDL2 pad state into the InputManager event queue

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