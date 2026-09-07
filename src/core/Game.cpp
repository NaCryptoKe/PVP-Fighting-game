#include "GL/glut.h"
#include <iostream>
#include <cstdlib>

#include "core/Game.h"

Game::Game() {}

void Game::init()
{
    timer.start();
    roundTimer.reset(90);

    player1.setHitBox(-100.0f, 0.0f, 200.0f, 300.0f);
    player2.setHitBox(-50.0f, 0.0f, 100.0f, 300.0f);

    player1.loadAttack(AttackType::LIGHT_PUNCH, 4, 3, 8, 50.0f, 100.0f, 150.0f, 120.0f, 80.0f, "LIGHT_PUNCH", 150.0f);
    player1.loadAttack(AttackType::HARD_PUNCH, 8, 4, 14, 120.0f, 120.0f, 150.0f, 160.0f, 100.0f, "HARD_PUNCH", 350.0f);

    player2.loadAttack(AttackType::LIGHT_PUNCH, 4, 3, 8, 50.0f, 100.0f, 150.0f, 120.0f, 80.0f, "LIGHT_PUNCH", 150.0f);
    player2.loadAttack(AttackType::HARD_PUNCH, 8, 4, 14, 120.0f, 120.0f, 150.0f, 160.0f, 100.0f, "HARD_PUNCH", 350.0f);
}

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    camera.apply(player1, player2);

    player1.render();
    player1.renderHitBox();
    if (player1.isActiveAttack()) player1.renderDamageBox(player1.getCurrentAttackName());

    player2.render();
    player2.renderHitBox();
    if (player2.isActiveAttack()) player2.renderDamageBox(player2.getCurrentAttackName());

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
    if (height == 0) height = 1;

    const float TARGET_ASPECT = 1920.0f / 1080.0f;
    float aspectRatio = (float)width / (float)height;

    int vpX = 0, vpY = 0, vpWidth = width, vpHeight = height;

    if (aspectRatio > TARGET_ASPECT)
    {
        vpWidth = static_cast<int>(height * TARGET_ASPECT);
        vpX = (width - vpWidth) / 2;
    }
    else
    {
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
  //printf("PLAYER 1: %.2f\nPLAYER 2: %.2f\n", player1.getHealth(), player2.getHealth());

    timer.update();
    float deltaTime = timer.getDeltaTime();

    if (matchPhase == MatchPhase::MATCH_OVER)
    {
        handleRematchExitInput();
        input.updateKeyState();
        return;
    }

    if (matchPhase == MatchPhase::ROUND_OVER)
    {
        roundEndTimer -= deltaTime;
        if (roundEndTimer <= 0.0f) resetRound();
        input.updateKeyState();
        return;
    }

    if (player1.getState() != PlayerState::ATTACK) player1.autoFace(player2.getPositionX());
    if (player2.getState() != PlayerState::ATTACK) player2.autoFace(player1.getPositionX());

    // ---- Player 1 ----
    if (input.isKeyPressed('u') && player1.canMove()) player1.performAttack("LIGHT_PUNCH");
    if (input.isKeyPressed('j') && player1.canMove()) player1.performAttack("HARD_PUNCH");

    player1.setBlocking(input.isKeyDown('k'));
    player1.setCrouching(input.isKeyDown('s'));

    if (player1.canMove())
    {
        bool moveLeftHeld  = input.isKeyDown('a') || input.isKeyDown('A');
        bool moveRightHeld = input.isKeyDown('d') || input.isKeyDown('D');

        if (moveLeftHeld)       { player1.isFacingRight() ? player1.moveBack()  : player1.moveFront(); }
        else if (moveRightHeld) { player1.isFacingRight() ? player1.moveFront() : player1.moveBack();  }
        else                    { player1.stopX(); }

        if (input.isKeyDown('w') || input.isKeyDown('W')) player1.jump();
    }

    // ---- Player 2 ----
    if (input.isKeyPressed('1') && player2.canMove()) player2.performAttack("LIGHT_PUNCH");
    if (input.isKeyPressed('2') && player2.canMove()) player2.performAttack("HARD_PUNCH");

    player2.setBlocking(input.isKeyDown('3'));
    player2.setCrouching(input.isSpecialKeyDown(GLUT_KEY_DOWN));

    if (player2.canMove())
    {
        bool moveLeftHeld  = input.isSpecialKeyDown(GLUT_KEY_LEFT);
        bool moveRightHeld = input.isSpecialKeyDown(GLUT_KEY_RIGHT);

        if (moveLeftHeld)       { player2.isFacingRight() ? player2.moveBack()  : player2.moveFront(); }
        else if (moveRightHeld) { player2.isFacingRight() ? player2.moveFront() : player2.moveBack();  }
        else                    { player2.stopX(); }

        if (input.isSpecialKeyDown(GLUT_KEY_UP)) player2.jump();
    }

    camera.updateBounds(player1, player2);

    player1.updateAttack(deltaTime);
    player2.updateAttack(deltaTime);

    player1.updateHitstun(deltaTime);
    player2.updateHitstun(deltaTime);

    player1.update(deltaTime);
    player2.update(deltaTime);

    player1.collision(Camera::STAGE_LEFT, Camera::STAGE_RIGHT);
    player2.collision(Camera::STAGE_LEFT, Camera::STAGE_RIGHT);

    resolvePlayerCollision();
    resolveCombat();

    if (player1.isDead() || player2.isDead())
    {
        if (player1.isDead()) roundWinsP2++;
        if (player2.isDead()) roundWinsP1++;

        std::cout << "Round over! Score - P1: " << roundWinsP1 << " P2: " << roundWinsP2 << "\n";

        if (roundWinsP1 >= 2 || roundWinsP2 >= 2)
        {
            matchPhase = MatchPhase::MATCH_OVER;
            std::cout << (roundWinsP1 >= 2 ? "PLAYER 1 WINS THE MATCH!" : "PLAYER 2 WINS THE MATCH!")
                      << " (R = rematch, Q = quit)\n";
        }
        else
        {
            matchPhase = MatchPhase::ROUND_OVER;
            roundEndTimer = 2.0f;
        }
    }

    input.updateKeyState();
    roundTimer.update(deltaTime);
}

void Game::resetRound()
{
    player1.resetForRound(480.0f, 120.0f);
    player2.resetForRound(1440.0f, 120.0f);
    matchPhase = MatchPhase::FIGHTING;
    roundTimer.reset(90);
}

void Game::handleRematchExitInput()
{
    if (input.isKeyPressed('r'))
    {
        roundWinsP1 = 0;
        roundWinsP2 = 0;
        resetRound();
    }
    else if (input.isKeyPressed('q'))
    {
        std::exit(0);
    }
}

void Game::keyboardDownCallback(unsigned char key, [[maybe_unused]]int x, [[maybe_unused]]int y) { input.handleKeyDown(key, x, y); }
void Game::keyboardUpCallback(unsigned char key, [[maybe_unused]]int x, [[maybe_unused]]int y) { input.handleKeyUp(key, x, y); }
void Game::specialKeyDownCallback(int key, [[maybe_unused]]int x, [[maybe_unused]]int y) { input.handleSpecialKeyDown(key, x, y); }
void Game::specialKeyUpCallback(int key, [[maybe_unused]]int x, [[maybe_unused]]int y) { input.handleSpecialKeyUp(key, x, y); }

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
            player2.onHit(player1.getCurrentAttackDamage(), player1.getCurrentAttackKnockback(), player1.isFacingRight());
            player1.markHit();
        }
    }

    if (player2.getActiveAttackHitbox(attackBox) && !player2.getHasHit())
    {
        if (BOX::intersects(attackBox, player1.getHitBox()))
        {
            player1.onHit(player2.getCurrentAttackDamage(), player2.getCurrentAttackKnockback(), player2.isFacingRight());
            player2.markHit();
        }
    }
}