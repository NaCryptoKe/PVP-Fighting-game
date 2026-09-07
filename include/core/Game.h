#ifndef GAME_H
#define GAME_H

#include "core/Timer.h"
#include "core/InputManager.h"
#include "entities/Character.h"
#include "utils/RoundTimer.h"
#include "entities/Camera.h"
#include "render/HUD.h"
#include "render/Font.h"

enum class MatchPhase { FIGHTING, ROUND_OVER, MATCH_OVER };

class Game {
public:
  Game();
  void init();

  void render();
  void reshape(int width, int height);
  void update();

  void keyboardDownCallback(unsigned char key, [[maybe_unused]]int x, [[maybe_unused]]int y);
  void keyboardUpCallback(unsigned char key, [[maybe_unused]]int x, [[maybe_unused]]int y);
  void specialKeyDownCallback(int key, [[maybe_unused]]int x, [[maybe_unused]]int y);
  void specialKeyUpCallback(int key, [[maybe_unused]]int x, [[maybe_unused]]int y);

  void resolveCharacterCollision();
  void resolveCombat();
  void endRound();
  void resetRound();
  void handleRematchExitInput();

private:
  Timer timer;
  InputManager input;

  Character character1{480.0f, 120.0f, 1000.0f};
  Character character2{1440.0f, 120.0f, 1000.0f};

  RoundTimer roundTimer;
  Camera camera;

  HUD hud1{character1, 200.0f};
  HUD hud2{character2, 200.0f};
  Font gameFont;

  int roundWinsP1 = 0;
  int roundWinsP2 = 0;
  // Who won the most recent round: 0 = none/tie, 1 = Player 1, 2 = Player 2
  int lastRoundWinner = 0;
  MatchPhase matchPhase = MatchPhase::FIGHTING;
  float roundEndTimer = 0.0f;
};

#endif  // GAME_H