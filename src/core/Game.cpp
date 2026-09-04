#include "GL/glut.h"
#include <iostream>

#include "core/Game.h"


std::string attackName = "hard hit";

Game::Game() {}

void Game::init()
{
  timer.start();

  roundTimer.reset(90);

  player1.setHitBox(-100.0f, 0.0, 200.0f, 300.0f);
  player2.setHitBox(-50.0f, 0.0f, 100.0f, 300.0f);

  player1.loadAttack(
        AttackType::LIGHT_PUNCH, 
        4, 3, 8,            
        50.0f,              
        100.0f, 150.0f,     
        120.0f, 80.0f,      
        "LIGHT_PUNCH", 
        150.0f              
    );

    player1.loadAttack(
        AttackType::HARD_PUNCH, 
        8, 4, 14, 
        120.0f, 
        120.0f, 150.0f, 
        160.0f, 100.0f, 
        "HARD_PUNCH", 
        350.0f
    );
}

void Game::render()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  camera.apply(player1, player2);

  player1.render();
  player1.renderHitBox();

  player2.render();
  player2.renderHitBox();

  player1.renderDamageBox("HARD_PUNCH");

  glLineWidth(3.0f);
  glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-2880.0f, 120.0f);
    glVertex2f(4800.0f, 120.0f);
  glEnd();
  
  glutSwapBuffers();
}

void Game::reshape(int width, int height)
{
    if (height == 0) height = 1;    // Preventing division by zero

    const float TARGET_ASPECT = 1920.0f / 1080.0f;
    float aspectRatio = (float)width / (float) height;

    int vpX = 0;
    int vpY = 0;
    int vpWidth = width;
    int vpHeight = height;

    if (aspectRatio > TARGET_ASPECT)
    {
      // Window is wider than 16:9 (Pillarboxing)
      vpWidth = static_cast<int>(height * TARGET_ASPECT);
      vpX = (width - vpWidth) / 2;
    }
    else
    {
      // Window is narrower/taller than 16:9 (Letterboxing)
      vpHeight = static_cast<int>(width / TARGET_ASPECT);
      vpY = (height - vpHeight) / 2;
    }

    glViewport(vpX, vpY, vpWidth, vpHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1920.0, 0.0, 1080.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
}

void Game::update()
{
  timer.update();
  float deltaTime = timer.getDeltaTime();
  player1.autoFace(player2.getPositionX());
  player2.autoFace(player1.getPositionX());

  if (input.isKeyPressed('j')) { player1.performAttack("HARD_PUNCH"); }

  if (player1.getState() != PlayerState::ATTACK)
  {
    // 1. Determine key states
    bool moveLeftHeld  = input.isKeyDown('a') || input.isKeyDown('A');
    bool moveRightHeld = input.isKeyDown('d') || input.isKeyDown('D');
    // 2. Relative Movement Logic
    if (moveLeftHeld) { player1.isFacingRight() ? player1.moveBack() : player1.moveFront(); }
    else if (moveRightHeld) { player1.isFacingRight() ? player1.moveFront() : player1.moveBack(); }
    else { player1.stopX(); }

    if (input.isKeyDown('w') || input.isKeyDown('W')) { player1.jump(); }
  }

  // if (!roundTimer.isExpired())
  // {
  //   if (currentTime != roundTimer.getSecondsRemaining())
  //   {
  //     std::cout << roundTimer.getSecondsRemaining() << "\n";
  //     currentTime = roundTimer.getSecondsRemaining();
  //   }
  // }

  camera.updateBounds(player1, player2);

  player1.updateAttack(deltaTime);

  player1.update(deltaTime);
  player2.update(deltaTime);

  player1.collision(Camera::STAGE_LEFT, Camera::STAGE_RIGHT);
  player2.collision(Camera::STAGE_LEFT, Camera::STAGE_RIGHT);

  resolvePlayerCollision();
  resolveCombat();

  input.updateKeyState();
  roundTimer.update(deltaTime);
}

void Game::keyboardDownCallback(unsigned char key, [[maybe_unused]]int x, [[maybe_unused]]int y) {
  input.handleKeyDown(key, x, y);
}

void Game::keyboardUpCallback(unsigned char key, [[maybe_unused]]int x, [[maybe_unused]]int y) {
  input.handleKeyUp(key, x, y);
}

void Game::specialKeyDownCallback(int key, [[maybe_unused]]int x, [[maybe_unused]]int y) {
  input.handleKeyDown(key, x, y);
}

void Game::specialKeyUpCallback(int key, [[maybe_unused]]int x, [[maybe_unused]]int y) {
  input.handleKeyUp(key, x, y);
}

void Game::resolvePlayerCollision()
{
  AABB player1HitBox = player1.getHitBox();
  AABB player2HitBox = player2.getHitBox();

  if (BOX::intersects(player1HitBox, player2HitBox))
  {
    float overlapX = 0.0f;

    if (player1.getPositionX() < player2.getPositionX()) 
    {
      overlapX = player1HitBox.right - player2HitBox.left;
      player1.setPositionX(player1.getPositionX() - overlapX * 0.5f);
      player2.setPositionX(player2.getPositionX() + overlapX * 0.5f);
    } 
    else 
    {
        overlapX = player2HitBox.right - player1HitBox.left;
        player1.setPositionX(player1.getPositionX() + overlapX * 0.5f);
        player2.setPositionX(player2.getPositionX() - overlapX * 0.5f);
    }
  }
}

void Game::resolveCombat()
{
    AABB attackBox;

    if (player1.getActiveAttackHitbox(attackBox) && !player1.getHasHit())
    {
        if (BOX::intersects(attackBox, player2.getHitBox()))
        {
            player2.takeDamage(player1.getCurrentAttackDamage());
            player1.markHit();
            player2.applyKnockback(player1.getCurrentAttackKnockback(), player1.isFacingRight());
        }
    }

    if (player2.getActiveAttackHitbox(attackBox) && !player2.getHasHit())
    {
        if (BOX::intersects(attackBox, player1.getHitBox()))
        {
            player1.takeDamage(player2.getCurrentAttackDamage());
            player2.markHit();
        }
    }
}