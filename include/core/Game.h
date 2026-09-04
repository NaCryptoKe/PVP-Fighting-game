#ifndef GAME_H
#define GAME_H

#include "core/Timer.h"
#include "core/InputManager.h"
#include "entities/Player.h"
#include "utils/RoundTimer.h"
#include "entities/Camera.h"

class Game {
public:
  Game();

  void init();

  void render();
  void reshape(int width, int height);
  void update();

  // Keyboard related input methods
  void keyboardDownCallback(unsigned char key, [[maybe_unused]]int x, [[maybe_unused]]int y);
  void keyboardUpCallback(unsigned char key, [[maybe_unused]]int x, [[maybe_unused]]int y);
  void specialKeyDownCallback(int key, [[maybe_unused]]int x, [[maybe_unused]]int y);
  void specialKeyUpCallback(int key, [[maybe_unused]]int x, [[maybe_unused]]int y);

  // Collision Detection and hurtboxes
  void resolvePlayerCollision();
  void resolveCombat();

  // Temporary
  int currentTime = 100;


private:
  Timer timer;

  InputManager input;

  Player player1{480.0f, 120.0f, 1000.0f};
  Player player2{1440.0f, 120.0f, 1000.0f};

  RoundTimer roundTimer;

  Camera camera;

};

#endif  // GAME_H
