#include "GL/glut.h"
#include <iostream>
#include <cstdlib>

#include "core/Game.h"
#include "render/Renderer.h"
#include "entities/CharacterRoster.h"

Game::Game() {}

void Game::init()
{
    Renderer::init();

    // Load character data from the roster — configure once, use everywhere
    for (const std::string& name : CharacterRoster::getAvailableCharacters())
        std::cout << "Roster: " << name << " ready\n";

    CharacterData data = CharacterRoster::getCharacter("Stickman");
    character1.applyData(data);
    character2.applyData(data);

    timer.start();
    roundTimer.reset(90);

    // Load font for UI
    gameFont.load("assets/fonts/main.ttf", 32.0f);
    hud1.setViewportSize(1920.0f, 1080.0f);
    hud2.setViewportSize(1920.0f, 1080.0f);
}

void Game::render()
{
    Renderer::clear(0.0f, 0.0f, 0.0f, 1.0f);
    glLoadIdentity();

    camera.apply(character1, character2);

    character1.render();
    character1.renderHitBox();
    if (character1.isActiveAttack()) character1.renderDamageBox(character1.getCurrentAttackName());

    character2.render();
    character2.renderHitBox();
    if (character2.isActiveAttack()) character2.renderDamageBox(character2.getCurrentAttackName());

    glLineWidth(3.0f);
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-2880.0f, 120.0f);
        glVertex2f(4800.0f, 120.0f);
    glEnd();

    // Switch to orthographic projection for HUD rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, 1920.0, 0.0, 1080.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Render HUD elements
    hud1.drawHealthBar(50.0f, 1000.0f, 30.0f, false);
    hud2.drawHealthBar(1670.0f, 1000.0f, 30.0f, true);

    // Round countdown timer is always visible during a fight
    if (matchPhase == MatchPhase::FIGHTING)
        hud1.drawTimer(gameFont, roundTimer.getSecondsRemaining(), 50.0f);
    
    if (matchPhase == MatchPhase::ROUND_OVER)
    {
        if (lastRoundWinner == 2)
            hud2.drawWinnerMessage(gameFont, "Player 2 Wins the Round!", "Round Over");
        else if (lastRoundWinner == 1)
            hud1.drawWinnerMessage(gameFont, "Player 1 Wins the Round!", "Round Over");
        else
            hud1.drawWinnerMessage(gameFont, "Round Over", "Tie - No Winner!");
    }
    
    if (matchPhase == MatchPhase::MATCH_OVER)
    {
        if (roundWinsP1 >= 2)
            hud1.drawWinnerMessage(gameFont, "Player 1 is Champion!", "Match Over - R rematch, Q quit");
        else
            hud2.drawWinnerMessage(gameFont, "Player 2 is Champion!", "Match Over - R rematch, Q quit");
    }

    // Restore projection matrix
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

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

    if (character1.getState() != CharacterState::ATTACK) character1.autoFace(character2.getPositionX());
    if (character2.getState() != CharacterState::ATTACK) character2.autoFace(character1.getPositionX());

    // ---- Player 1 ----
    if (input.isKeyPressed('u') && character1.canMove()) character1.performAttack("LIGHT_PUNCH");
    if (input.isKeyPressed('j') && character1.canMove()) character1.performAttack("HARD_PUNCH");

    character1.setBlocking(input.isKeyDown('k'));
    character1.setCrouching(input.isKeyDown('s'));

    if (character1.canMove())
    {
        bool moveLeftHeld  = input.isKeyDown('a') || input.isKeyDown('A');
        bool moveRightHeld = input.isKeyDown('d') || input.isKeyDown('D');

        if (moveLeftHeld)       { character1.isFacingRight() ? character1.moveBack()  : character1.moveFront(); }
        else if (moveRightHeld) { character1.isFacingRight() ? character1.moveFront() : character1.moveBack();  }
        else                    { character1.stopX(); }

        if (input.isKeyDown('w') || input.isKeyDown('W')) character1.jump();
    }

    // ---- Player 2 ----
    if (input.isKeyPressed('1') && character2.canMove()) character2.performAttack("LIGHT_PUNCH");
    if (input.isKeyPressed('2') && character2.canMove()) character2.performAttack("HARD_PUNCH");

    character2.setBlocking(input.isKeyDown('3'));
    character2.setCrouching(input.isSpecialKeyDown(GLUT_KEY_DOWN));

    if (character2.canMove())
    {
        bool moveLeftHeld  = input.isSpecialKeyDown(GLUT_KEY_LEFT);
        bool moveRightHeld = input.isSpecialKeyDown(GLUT_KEY_RIGHT);

        if (moveLeftHeld)       { character2.isFacingRight() ? character2.moveBack()  : character2.moveFront(); }
        else if (moveRightHeld) { character2.isFacingRight() ? character2.moveFront() : character2.moveBack();  }
        else                    { character2.stopX(); }

        if (input.isSpecialKeyDown(GLUT_KEY_UP)) character2.jump();
    }

    camera.updateBounds(character1, character2);

    character1.updateAttack(deltaTime);
    character2.updateAttack(deltaTime);

    character1.updateHitstun(deltaTime);
    character2.updateHitstun(deltaTime);

    character1.update(deltaTime);
    character2.update(deltaTime);

    character1.collision(Camera::STAGE_LEFT, Camera::STAGE_RIGHT);
    character2.collision(Camera::STAGE_LEFT, Camera::STAGE_RIGHT);

    resolveCharacterCollision();
    resolveCombat();

    // Round timer ran out → higher health wins the round
    if (roundTimer.isExpired())
    {
        if (character1.getHealth() > character2.getHealth())      { lastRoundWinner = 1; roundWinsP1++; }
        else if (character2.getHealth() > character1.getHealth()) { lastRoundWinner = 2; roundWinsP2++; }
        else                                                       { lastRoundWinner = 0; }

        endRound();
        input.updateKeyState();
        roundTimer.update(deltaTime);
        return;
    }

    if (character1.isDead() || character2.isDead())
    {
        if (character1.isDead()) { lastRoundWinner = 2; roundWinsP2++; }
        if (character2.isDead()) { lastRoundWinner = 1; roundWinsP1++; }

        endRound();
    }

    input.updateKeyState();
    roundTimer.update(deltaTime);
}

void Game::endRound()
{
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

void Game::resetRound()
{
    character1.resetForRound(480.0f, 120.0f);
    character2.resetForRound(1440.0f, 120.0f);
    matchPhase = MatchPhase::FIGHTING;
    roundTimer.reset(90);   // every round starts with 90 seconds
    lastRoundWinner = 0;
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

void Game::resolveCharacterCollision()
{
    AABB char1HitBox = character1.getHitBox();
    AABB char2HitBox = character2.getHitBox();

    if (BOX::intersects(char1HitBox, char2HitBox))
    {
        float overlapX = 0.0f;
        if (character1.getPositionX() < character2.getPositionX())
        {
            overlapX = char1HitBox.right - char2HitBox.left;
            character1.setPositionX(character1.getPositionX() - overlapX * 0.5f);
            character2.setPositionX(character2.getPositionX() + overlapX * 0.5f);
        }
        else
        {
            overlapX = char2HitBox.right - char1HitBox.left;
            character1.setPositionX(character1.getPositionX() + overlapX * 0.5f);
            character2.setPositionX(character2.getPositionX() - overlapX * 0.5f);
        }
    }
}

void Game::resolveCombat()
{
    AABB attackBox;

    if (character1.getActiveAttackHitbox(attackBox) && !character1.getHasHit())
    {
        if (BOX::intersects(attackBox, character2.getHitBox()))
        {
            character2.onHit(character1.getCurrentAttackDamage(), character1.getCurrentAttackKnockback(), character1.isFacingRight());
            character1.markHit();
        }
    }

    if (character2.getActiveAttackHitbox(attackBox) && !character2.getHasHit())
    {
        if (BOX::intersects(attackBox, character1.getHitBox()))
        {
            character1.onHit(character2.getCurrentAttackDamage(), character2.getCurrentAttackKnockback(), character2.isFacingRight());
            character2.markHit();
        }
    }
}