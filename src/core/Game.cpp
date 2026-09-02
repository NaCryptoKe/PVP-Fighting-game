#include "GL/glut.h"
#include <iostream>

#include "core/Game.h"
Game::Game() {}

void Game::init()
{
  timer.start();

  roundTimer.reset(90);

  player1.setHitBox(-0.09f, 0.0,0.18f, 0.15f);
  player2.setHitBox(-0.05f, 0.0f, 0.10f, 0.15f);
}

void Game::render()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  player1.render();
  player1.renderHitBox();

  player2.render();
  player2.renderHitBox();
  
  glutSwapBuffers();
}

void Game::reshape(int width, int height)
{
    if (height == 0) height = 1;    // Preventing division by zero
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION); // Must re-implement this by my own, builtin functions aren't allowed
    glLoadIdentity();

    float aspect = (float)width / (float)height;
    if (width >= height)
    {
      glOrtho(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0, -1.0, 1.0); // Must implement ortho by my self
    }
    else
    {
      glOrtho(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect, -1.0, 1.0);
    }

    glMatrixMode(GL_MODELVIEW);
}

void Game::update()
{
  timer.update();
  float deltaTime = timer.getDeltaTime();
  player1.render();

  bool facingRight = player1.isFacingRight();

  // 1. Determine key states
  bool moveLeftHeld  = input.isKeyDown('a') || input.isKeyDown('A');
  bool moveRightHeld = input.isKeyDown('d') || input.isKeyDown('D');

  // 2. Relative Movement Logic
  if (moveLeftHeld) 
  {
    player1.moveBack();
  }
  else if (moveRightHeld) 
  {
    player1.moveFront();
  }
  else 
  {
      player1.stopX();
  }

  if (input.isKeyPressed('w') || input.isKeyPressed('W')) 
  {
    player1.jump();
  }

  if (input.isKeyReleased(' ')) 
  {
      player1.setFacingRight(!facingRight);
  }

  if (input.isKeyDown('y')) player1.setState(PlayerState::IDLE);
  if (input.isKeyDown('u')) player1.setState(PlayerState::WALK);
  if (input.isKeyDown('i')) player1.setState(PlayerState::RUN);
  if (input.isKeyDown('o')) player1.setState(PlayerState::JUMP);
  if (input.isKeyDown('p')) player1.setState(PlayerState::ATTACK);
  if (input.isKeyDown('[')) player1.setState(PlayerState::BLOCK);

  if (!roundTimer.isExpired())
  {
    if (currentTime != roundTimer.getSecondsRemaining())
    {
      std::cout << roundTimer.getSecondsRemaining() << "\n";
      currentTime = roundTimer.getSecondsRemaining();
    }
  }
  player1.update(deltaTime);
  player2.update(deltaTime);
  resolvePlayerCollision();
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