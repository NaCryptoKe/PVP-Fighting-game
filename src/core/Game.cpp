#include "GL/glut.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "core/Game.h"
#include "render/Renderer.h"
#include "entities/CharacterRoster.h"
#include "computer_graphics/core/Orthographic.h"
#include "computer_graphics/core/Transform.h"

Game::Game() {}

void Game::init()
{
    Renderer::init();

    // Load character data from the roster — configure once, use everywhere
    for (const std::string& name : CharacterRoster::getAvailableCharacters())
        std::cout << "Roster: " << name << " ready\n";

    CharacterData data = CharacterRoster::getCharacter("Ryu");
    character1.applyData(data);
    character2.applyData(data);

    // Fireball projectile: textures are loaded once and reused for every spawn
    projectileTemplate.loadFromFiles("assets/projectiles/blue_fireball/", 4, 0.06f);

    // Parallax background layers (sky, mountains, playfield wall)
    background.load();

    timer.start();
    roundTimer.reset(90);

    // Load font for UI
    gameFont.load("assets/fonts/main.ttf", 32.0f);
    hud1.setViewportSize(1920.0f, 1080.0f);
    hud2.setViewportSize(1920.0f, 1080.0f);

    // End-of-match menu buttons (1920x1080 virtual HUD space)
    buttons[0] = { 660.0f, 280.0f, 260.0f, 90.0f, "REMATCH" };
    buttons[1] = { 1000.0f, 280.0f, 260.0f, 90.0f, "QUIT" };

    printControls();
}

void Game::printControls() const
{
    std::cout << "\n==================== CONTROLS ====================\n"
              << "  Player 1                |  Player 2\n"
              << "  A/D move, W jump        |  Left/Right move, Up jump\n"
              << "  S crouch, G block       |  Down crouch, 7 block\n"
              << "  U light punch           |  1 light punch\n"
              << "  I light kick            |  2 light kick\n"
              << "  J hard punch            |  3 hard punch\n"
              << "  K hard kick             |  4 hard kick\n"
              << "  L fireball              |  5 fireball\n"
              << "  H throw                 |  6 throw\n"
              << "  Hold 0: show hurtboxes  |  (works for both)\n"
              << "==================================================\n\n";
}

void Game::render()
{
    Renderer::clear(0.35f, 0.35f, 0.35f, 1.0f);   // gray background
    glLoadIdentity();

    camera.apply(character1, character2);

    // Parallax background: drawn in world space, behind everything else
    background.render(camera);

    // Hurtboxes are a debug view: hidden unless 0 is held down
    bool showHurtboxes = input.isKeyDown('0');

    character1.render();
    if (showHurtboxes) character1.renderHitBox();
    if (character1.isActiveAttack()) character1.renderDamageBox(character1.getCurrentAttackName());

    character2.render();
    if (showHurtboxes) character2.renderHitBox();
    if (character2.isActiveAttack()) character2.renderDamageBox(character2.getCurrentAttackName());

    // Fireballs in flight
    for (const Projectile& projectile : projectiles)
        projectile.render();

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
    // Custom orthographic projection from the cg math core (glOrtho equivalent)
    glLoadMatrixf(cg::Orthographic(0.0f, 1920.0f, 0.0f, 1080.0f).toMatrix().data());
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Render HUD elements
    hud1.drawHealthBar(50.0f, 1000.0f, 30.0f, false);
    hud2.drawHealthBar(1670.0f, 1000.0f, 30.0f, true);

    // Combo counters flash on the attacker's half of the screen
    if (p1ComboHits >= 2 && p1ComboTimer > 0.0f) drawComboText(480.0f, p1ComboHits);
    if (p2ComboHits >= 2 && p2ComboTimer > 0.0f) drawComboText(1440.0f, p2ComboHits);

    // Interactive tutorial before the first fight
    if (matchPhase == MatchPhase::TUTORIAL)
        drawTutorial();

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
            hud1.drawWinnerMessage(gameFont, "Player 1 is Champion!", "Match Over");
        else
            hud2.drawWinnerMessage(gameFont, "Player 2 is Champion!", "Match Over");

        // Clickable end-of-match menu
        drawMenuButtons();
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

    // Remember the mapping so mouse clicks can be converted to HUD space
    viewportX = static_cast<float>(vpX);
    viewportY = static_cast<float>(vpY);
    viewportWidth = static_cast<float>(vpWidth);
    viewportHeight = static_cast<float>(vpHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Custom orthographic projection from the cg math core (glOrtho equivalent)
    glLoadMatrixf(cg::Orthographic(0.0f, 1920.0f, 0.0f, 1080.0f).toMatrix().data());
    glMatrixMode(GL_MODELVIEW);
}

void Game::update()
{
    timer.update();
    float deltaTime = timer.getDeltaTime();
    gameTime += deltaTime;

    if (matchPhase == MatchPhase::TUTORIAL)
    {
        // Space / Enter advances a page, ESC skips the whole tutorial
        if (input.isKeyPressed(' ') || input.isKeyPressed(13)) advanceTutorial();
        else if (input.isKeyPressed(27)) startFight();

        // Keep the fighters facing each other while they idle
        character1.autoFace(character2.getPositionX());
        character2.autoFace(character1.getPositionX());
    }
    else if (matchPhase == MatchPhase::ROUND_OVER)
    {
        roundEndTimer -= deltaTime;
        if (roundEndTimer <= 0.0f) resetRound();
    }
    else if (matchPhase == MatchPhase::FIGHTING)
    {
        updateFighting(deltaTime);
    }
    // MATCH_OVER: combat input is ignored; the fighters keep animating below
    // (champion celebration / KO lie-down)

    // Keep every animation alive in all phases
    character1.update(deltaTime);
    character2.update(deltaTime);
    updateProjectiles(deltaTime);

    // Combo text fades out in every phase
    p1ComboTimer = std::max(0.0f, p1ComboTimer - deltaTime);
    p2ComboTimer = std::max(0.0f, p2ComboTimer - deltaTime);

    input.updateKeyState();
}

void Game::updateFighting(float deltaTime)
{
    if (character1.getState() != CharacterState::ATTACK) character1.autoFace(character2.getPositionX());
    if (character2.getState() != CharacterState::ATTACK) character2.autoFace(character1.getPositionX());

    // ---- Player 1 ----
    // Attack presses are recorded in the circular input buffer so they can
    // be linked as soon as the fighter can act again (combo buffering)
    if (input.isKeyPressed('u')) p1Inputs.push(InputToken::LIGHT_PUNCH, gameTime);
    if (input.isKeyPressed('i')) p1Inputs.push(InputToken::LIGHT_KICK,  gameTime);
    if (input.isKeyPressed('j')) p1Inputs.push(InputToken::HARD_PUNCH,  gameTime);
    if (input.isKeyPressed('k')) p1Inputs.push(InputToken::HARD_KICK,   gameTime);
    if (input.isKeyPressed('l')) p1Inputs.push(InputToken::FIREBALL,    gameTime);
    if (input.isKeyPressed('h')) p1Inputs.push(InputToken::THROW,       gameTime);

    character1.updateGuard(input.isKeyDown('g'), input.isKeyDown('s'));

    if (character1.canMove())
    {
        bool moveLeftHeld  = input.isKeyDown('a') || input.isKeyDown('A');
        bool moveRightHeld = input.isKeyDown('d') || input.isKeyDown('D');

        if (moveLeftHeld)       { character1.isFacingRight() ? character1.moveBack()  : character1.moveFront(); }
        else if (moveRightHeld) { character1.isFacingRight() ? character1.moveFront() : character1.moveBack();  }
        else                    { character1.stopX(); }

        if (input.isKeyDown('w') || input.isKeyDown('W')) character1.jump();
    }

    // Fire the oldest buffered attack once the fighter can act again
    if (character1.canAct() && !character1.isBlocking())
    {
        InputToken token;
        if (p1Inputs.popOldest(token, gameTime, INPUT_BUFFER_WINDOW))
            executeBufferedAttack(character1, token);
    }

    // ---- Player 2 ----
    if (input.isKeyPressed('1')) p2Inputs.push(InputToken::LIGHT_PUNCH, gameTime);
    if (input.isKeyPressed('2')) p2Inputs.push(InputToken::LIGHT_KICK,  gameTime);
    if (input.isKeyPressed('3')) p2Inputs.push(InputToken::HARD_PUNCH,  gameTime);
    if (input.isKeyPressed('4')) p2Inputs.push(InputToken::HARD_KICK,   gameTime);
    if (input.isKeyPressed('5')) p2Inputs.push(InputToken::FIREBALL,    gameTime);
    if (input.isKeyPressed('6')) p2Inputs.push(InputToken::THROW,       gameTime);

    character2.updateGuard(input.isKeyDown('7'), input.isSpecialKeyDown(GLUT_KEY_DOWN));

    if (character2.canMove())
    {
        bool moveLeftHeld  = input.isSpecialKeyDown(GLUT_KEY_LEFT);
        bool moveRightHeld = input.isSpecialKeyDown(GLUT_KEY_RIGHT);

        if (moveLeftHeld)       { character2.isFacingRight() ? character2.moveBack()  : character2.moveFront(); }
        else if (moveRightHeld) { character2.isFacingRight() ? character2.moveFront() : character2.moveBack();  }
        else                    { character2.stopX(); }

        if (input.isSpecialKeyDown(GLUT_KEY_UP)) character2.jump();
    }

    // Fire the oldest buffered attack once the fighter can act again
    if (character2.canAct() && !character2.isBlocking())
    {
        InputToken token;
        if (p2Inputs.popOldest(token, gameTime, INPUT_BUFFER_WINDOW))
            executeBufferedAttack(character2, token);
    }

    camera.updateBounds(character1, character2);

    // Fireballs leave the fighter's hands when their startup frames finish
    if (character1.consumeProjectileSpawn()) spawnProjectile(character1, 1);
    if (character2.consumeProjectileSpawn()) spawnProjectile(character2, 2);

    character1.updateAttack(deltaTime);
    character2.updateAttack(deltaTime);

    character1.updateHitstun(deltaTime);
    character2.updateHitstun(deltaTime);

    character1.collision(Camera::STAGE_LEFT, Camera::STAGE_RIGHT);
    character2.collision(Camera::STAGE_LEFT, Camera::STAGE_RIGHT);

    resolveCharacterCollision();
    resolveCombat();

    // A combo ends once its victim recovers from hitstun
    if (character2.getState() != CharacterState::HITSTUN) p1ComboHits = 0;
    if (character1.getState() != CharacterState::HITSTUN) p2ComboHits = 0;

    // Round timer ran out → higher health wins the round
    if (roundTimer.isExpired())
    {
        if (character1.getHealth() > character2.getHealth())      { lastRoundWinner = 1; roundWinsP1++; }
        else if (character2.getHealth() > character1.getHealth()) { lastRoundWinner = 2; roundWinsP2++; }
        else                                                       { lastRoundWinner = 0; }

        endRound();
        roundTimer.update(deltaTime);
        return;
    }

    if (character1.isDead() || character2.isDead())
    {
        if (character1.isDead()) { lastRoundWinner = 2; roundWinsP2++; }
        if (character2.isDead()) { lastRoundWinner = 1; roundWinsP1++; }

        endRound();
    }

    roundTimer.update(deltaTime);
}

// ----------------------------------------------------------------
// Mouse menu (clickable end-of-match buttons)
// ----------------------------------------------------------------

void Game::onMouseButton(int button, int state, int x, int y)
{
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;

    float virtualX = 0.0f, virtualY = 0.0f;
    windowToVirtual(x, y, virtualX, virtualY);

    if (matchPhase == MatchPhase::TUTORIAL)
    {
        if (isInsideButton(tutorialButtons[0], virtualX, virtualY))      advanceTutorial();
        else if (isInsideButton(tutorialButtons[1], virtualX, virtualY)) startFight();
        return;
    }

    if (matchPhase != MatchPhase::MATCH_OVER) return;

    if (isInsideButton(buttons[0], virtualX, virtualY))      startRematch();
    else if (isInsideButton(buttons[1], virtualX, virtualY)) std::exit(0);
}

void Game::onMouseMove(int x, int y)
{
    windowToVirtual(x, y, mouseVirtualX, mouseVirtualY);
}

void Game::windowToVirtual(int windowX, int windowY, float& virtualX, float& virtualY) const
{
    if (viewportWidth <= 0.0f || viewportHeight <= 0.0f)
    {
        virtualX = static_cast<float>(windowX);
        virtualY = 1080.0f - static_cast<float>(windowY);
        return;
    }

    virtualX = (windowX - viewportX) * (1920.0f / viewportWidth);
    virtualY = 1080.0f - (windowY - viewportY) * (1080.0f / viewportHeight);
}

bool Game::isInsideButton(const UIButton& button, float x, float y) const
{
    return x >= button.x && x <= button.x + button.width &&
           y >= button.y && y <= button.y + button.height;
}

void Game::startRematch()
{
    roundWinsP1 = 0;
    roundWinsP2 = 0;
    resetRound();
}

// Shared button renderer: dark panel, amber when the mouse hovers it
void Game::drawButton(const UIButton& button)
{
    float textHeight = gameFont.getTextHeight();
    bool hovered = isInsideButton(button, mouseVirtualX, mouseVirtualY);

    if (hovered)
        Renderer::drawQuad(button.x, button.y, button.width, button.height,
                           0.95f, 0.75f, 0.20f, 0.9f);      // amber highlight
    else
        Renderer::drawQuad(button.x, button.y, button.width, button.height,
                           0.10f, 0.10f, 0.12f, 0.85f);     // dark panel

    float textWidth = gameFont.getTextWidth(button.label.c_str());
    float textX = button.x + (button.width - textWidth) / 2.0f;
    float textY = button.y + (button.height - textHeight) / 2.0f + textHeight * 0.35f;

    gameFont.renderText(button.label.c_str(), textX, textY,
                        hovered ? 0.10f : 1.0f,
                        hovered ? 0.10f : 1.0f,
                        hovered ? 0.10f : 1.0f);
}

void Game::drawMenuButtons()
{
    for (const UIButton& button : buttons)
        drawButton(button);
}

// ----------------------------------------------------------------
// Tutorial (shown before the first fight)
// ----------------------------------------------------------------

void Game::advanceTutorial()
{
    tutorialPage++;
    if (tutorialPage >= TUTORIAL_PAGE_COUNT) startFight();
}

void Game::startFight()
{
    tutorialPage = 0;
    resetRound();   // also clears input buffers, combos and projectiles
}

void Game::drawTutorial()
{
    struct TutorialPage
    {
        std::string title;
        std::vector<std::string> lines;
    };

    static const std::vector<TutorialPage> pages = {
        { "MOVEMENT", {
            "Player 1:   A / D walk      W jump      S crouch",
            "Player 2:   Arrow keys to move, jump and crouch",
            "",
            "Hold 0 at any time to reveal the hurtboxes" } },
        { "ATTACKS", {
            "Player 1:   U light punch      I light kick",
            "             J hard punch      K hard kick",
            "Player 2:   1 light punch      2 light kick",
            "             3 hard punch      4 hard kick",
            "",
            "Attack while crouching or jumping for",
            "low and air attacks" } },
        { "COMBOS", {
            "Pressed attacks are stored in an input buffer,",
            "so the next hit can be linked as soon as the",
            "fighter can act again.",
            "",
            "Hits that land while the opponent is stunned",
            "build a COMBO - each extra hit deals less damage" } },
        { "DEFENSE & SPECIALS", {
            "Block: hold G (P1) or 7 (P2)",
            "Crouch + block guards low (crouch block)",
            "Blocked hits only chip and cause blockstun",
            "",
            "Throws (H / 6) are unblockable",
            "Fireballs (L / 5) fly across the stage" } },
    };

    const TutorialPage& page = pages[tutorialPage];

    // Panel over the idle fighters
    Renderer::drawQuad(330.0f, 150.0f, 1260.0f, 800.0f, 0.07f, 0.07f, 0.09f, 0.90f);

    // Title
    float titleWidth = gameFont.getTextWidth(page.title.c_str());
    gameFont.renderText(page.title.c_str(), 960.0f - titleWidth / 2.0f, 860.0f,
                        1.0f, 0.75f, 0.20f);

    // Spinning diamond accents — live demo of cg::Rotate: a colored quad
    // whose Transform rotates continuously around its own center.
    const float spin = gameTime * 90.0f;
    cg::Transform diamondLeft;
    diamondLeft.position = cg::Vec3(430.0f, 860.0f, 0.0f);
    diamondLeft.rotationDegrees = spin;
    diamondLeft.scale = cg::Vec3(26.0f, 26.0f, 1.0f);
    Renderer::drawColoredQuad(diamondLeft, 1.0f, 0.75f, 0.20f, 0.9f);

    cg::Transform diamondRight;
    diamondRight.position = cg::Vec3(1490.0f, 860.0f, 0.0f);
    diamondRight.rotationDegrees = spin + 45.0f;
    diamondRight.scale = cg::Vec3(26.0f, 26.0f, 1.0f);
    Renderer::drawColoredQuad(diamondRight, 1.0f, 0.75f, 0.20f, 0.9f);

    // Body lines
    float lineY = 780.0f;
    for (const std::string& line : page.lines)
    {
        if (!line.empty())
        {
            float lineWidth = gameFont.getTextWidth(line.c_str());
            gameFont.renderText(line.c_str(), 960.0f - lineWidth / 2.0f, lineY,
                                1.0f, 1.0f, 1.0f);
        }
        lineY -= 55.0f;
    }

    // Page counter
    std::string counter = "Page " + std::to_string(tutorialPage + 1) + " / " +
                          std::to_string(TUTORIAL_PAGE_COUNT);
    float counterWidth = gameFont.getTextWidth(counter.c_str());
    gameFont.renderText(counter.c_str(), 960.0f - counterWidth / 2.0f, 195.0f,
                        0.7f, 0.7f, 0.7f);

    // Navigation buttons (NEXT becomes START FIGHT on the last page)
    bool lastPage = (tutorialPage == TUTORIAL_PAGE_COUNT - 1);
    tutorialButtons[0] = { 660.0f, 250.0f, 260.0f, 80.0f, lastPage ? "START FIGHT" : "NEXT" };
    tutorialButtons[1] = { 1000.0f, 250.0f, 260.0f, 80.0f, "SKIP" };
    drawButton(tutorialButtons[0]);
    drawButton(tutorialButtons[1]);
}

// Maps a buffered input token to its attack
void Game::executeBufferedAttack(Character& character, InputToken token)
{
    switch (token)
    {
        case InputToken::LIGHT_PUNCH: character.performAttack("LIGHT_PUNCH"); break;
        case InputToken::LIGHT_KICK:  character.performAttack("LIGHT_KICK");  break;
        case InputToken::HARD_PUNCH:  character.performAttack("HARD_PUNCH");  break;
        case InputToken::HARD_KICK:   character.performAttack("HARD_KICK");   break;
        case InputToken::FIREBALL:    character.performAttack("FIREBALL");    break;
        case InputToken::THROW:       character.performAttack("THROW");       break;
        default: break;
    }
}

// Combo counter text on the attacker's half of the screen
void Game::drawComboText(float centerX, int hits)
{
    std::string text = std::to_string(hits) + " HIT COMBO";
    float textWidth = gameFont.getTextWidth(text.c_str());
    gameFont.renderText(text.c_str(), centerX - textWidth / 2.0f, 820.0f,
                        1.0f, 0.75f, 0.10f);
}

// ----------------------------------------------------------------
// Rounds & combat
// ----------------------------------------------------------------

void Game::endRound()
{
    std::cout << "Round over! Score - P1: " << roundWinsP1 << " P2: " << roundWinsP2 << "\n";
    projectiles.clear();

    bool matchOver = (roundWinsP1 >= 2 || roundWinsP2 >= 2);

    // Winner celebrates; the loser stays where they are (KO lie-down or idle)
    if (lastRoundWinner == 1 && !character1.isDead()) character1.setVictory(matchOver);
    if (lastRoundWinner == 2 && !character2.isDead()) character2.setVictory(matchOver);

    if (matchOver)
    {
        matchPhase = MatchPhase::MATCH_OVER;
        std::cout << (roundWinsP1 >= 2 ? "PLAYER 1 WINS THE MATCH!" : "PLAYER 2 WINS THE MATCH!")
                  << " - use the on-screen buttons\n";
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
    projectiles.clear();

    // Fresh input buffers and combo counters every round
    p1Inputs.clear();
    p2Inputs.clear();
    p1ComboHits = 0;
    p2ComboHits = 0;
    p1ComboTimer = 0.0f;
    p2ComboTimer = 0.0f;

    matchPhase = MatchPhase::FIGHTING;
    roundTimer.reset(90);   // every round starts with 90 seconds
    lastRoundWinner = 0;
}

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
    for (int attackerId = 1; attackerId <= 2; ++attackerId)
    {
        Character& attacker = (attackerId == 1) ? character1 : character2;
        Character& defender = (attackerId == 1) ? character2 : character1;

        if (!attacker.isActiveAttack() || attacker.getHasHit()) continue;

        AABB attackBox;
        if (!attacker.getActiveAttackHitbox(attackBox)) continue;
        if (!BOX::intersects(attackBox, defender.getHitBox())) continue;

        HitImpact impact;
        impact.damage      = attacker.getCurrentAttackDamage();
        impact.knockback   = attacker.getCurrentAttackKnockback();
        impact.pushRight   = attacker.isFacingRight();
        impact.hitstunTime = attacker.getCurrentAttackHitstun();
        impact.blockable   = attacker.isCurrentAttackBlockable();

        // Throws always connect — the victim is thrown backwards
        if (!impact.blockable) impact.animOverride = "BACKWARD_THROW";

        // Combo tracking: hits during hitstun extend the chain, blocked hits
        // reset it. Later hits in a chain deal scaled-down damage.
        int&   comboHits  = (attackerId == 1) ? p1ComboHits  : p2ComboHits;
        float& comboTimer = (attackerId == 1) ? p1ComboTimer : p2ComboTimer;

        if (defender.isBlocking() && impact.blockable)
        {
            comboHits = 0;
        }
        else
        {
            comboHits = (defender.getState() == CharacterState::HITSTUN) ? comboHits + 1 : 1;
            comboTimer = 1.2f;
            impact.damage *= std::max(0.4f, 1.0f - 0.1f * static_cast<float>(comboHits - 1));
        }

        defender.onHit(impact);
        attacker.markHit();
    }
}

void Game::updateProjectiles(float deltaTime)
{
    for (Projectile& projectile : projectiles)
    {
        if (!projectile.isActive()) continue;

        Character& opponent = (projectile.getOwner() == 1) ? character2 : character1;
        projectile.update(deltaTime, opponent);
    }

    // Remove fireballs that hit something or flew off the stage
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
                       [](const Projectile& p) { return !p.isActive(); }),
        projectiles.end());
}

void Game::spawnProjectile(Character& owner, int ownerId)
{
    float direction = owner.isFacingRight() ? 1.0f : -1.0f;

    projectiles.push_back(projectileTemplate);
    projectiles.back().spawn(
        owner.getPositionX() + direction * 120.0f,
        owner.getPositionY() + 130.0f,
        direction, ownerId);
}

void Game::keyboardDownCallback(unsigned char key, [[maybe_unused]]int x, [[maybe_unused]]int y) { input.handleKeyDown(key, x, y); }
void Game::keyboardUpCallback(unsigned char key, [[maybe_unused]]int x, [[maybe_unused]]int y) { input.handleKeyUp(key, x, y); }
void Game::specialKeyDownCallback(int key, [[maybe_unused]]int x, [[maybe_unused]]int y) { input.handleSpecialKeyDown(key, x, y); }
void Game::specialKeyUpCallback(int key, [[maybe_unused]]int x, [[maybe_unused]]int y) { input.handleSpecialKeyUp(key, x, y); }
