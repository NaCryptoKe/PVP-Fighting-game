#ifndef GAME_H
#define GAME_H

#include "core/Timer.h"
#include "core/InputManager.h"
#include "core/InputBuffer.h"
#include "entities/Character.h"
#include "entities/Projectile.h"
#include "entities/Background.h"
#include "utils/RoundTimer.h"
#include "entities/Camera.h"
#include "render/HUD.h"
#include "render/Font.h"

#include <string>
#include <vector>

enum class MatchPhase { TUTORIAL, FIGHTING, ROUND_OVER, MATCH_OVER };

// A clickable menu button in the 1920x1080 virtual HUD space
struct UIButton
{
    float x;
    float y;
    float width;
    float height;
    std::string label;
};

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
  void onMouseButton(int button, int state, int x, int y);
  void onMouseMove(int x, int y);

  void resolveCharacterCollision();
  void resolveCombat();
  void endRound();
  void resetRound();

private:
  Timer timer;
  InputManager input;

  Character character1{480.0f, 120.0f, 1000.0f};
  Character character2{1440.0f, 120.0f, 1000.0f};

  // Fireball template: textures loaded once, every spawn copies it
  Projectile projectileTemplate;
  std::vector<Projectile> projectiles;

  // Three-layer parallax background (sky, mountains, playfield wall)
  Background background;

  RoundTimer roundTimer;
  Camera camera;

  HUD hud1{character1, 200.0f};
  HUD hud2{character2, 200.0f};
  Font gameFont;

  // End-of-match menu (clickable) + mouse position in virtual HUD space
  UIButton buttons[2];
  float mouseVirtualX = -1.0f;
  float mouseVirtualY = -1.0f;

  // Interactive tutorial shown before the first fight
  UIButton tutorialButtons[2];
  int tutorialPage = 0;
  static constexpr int TUTORIAL_PAGE_COUNT = 4;

  // Circular input buffers (combo linking) and combo counters
  InputBuffer p1Inputs;
  InputBuffer p2Inputs;
  float gameTime = 0.0f;
  static constexpr float INPUT_BUFFER_WINDOW = 0.25f;
  int p1ComboHits = 0;
  int p2ComboHits = 0;
  float p1ComboTimer = 0.0f;
  float p2ComboTimer = 0.0f;

  // Window -> virtual HUD space mapping (kept by reshape)
  float viewportX = 0.0f;
  float viewportY = 0.0f;
  float viewportWidth = 1920.0f;
  float viewportHeight = 1080.0f;

  int roundWinsP1 = 0;
  int roundWinsP2 = 0;
  // Who won the most recent round: 0 = none/tie, 1 = Player 1, 2 = Player 2
  int lastRoundWinner = 0;
  MatchPhase matchPhase = MatchPhase::TUTORIAL;
  float roundEndTimer = 0.0f;

  void updateFighting(float deltaTime);
  void updateProjectiles(float deltaTime);
  void spawnProjectile(Character& owner, int ownerId);
  void drawMenuButtons();
  void drawButton(const UIButton& button);
  void drawTutorial();
  void drawComboText(float centerX, int hits);
  void executeBufferedAttack(Character& character, InputToken token);
  void advanceTutorial();
  void startFight();
  void startRematch();
  bool isInsideButton(const UIButton& button, float x, float y) const;
  void windowToVirtual(int windowX, int windowY, float& virtualX, float& virtualY) const;
  void printControls() const;
};

#endif  // GAME_H
