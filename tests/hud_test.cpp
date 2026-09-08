#ifndef HUD_TEST_H
#define HUD_TEST_H

/* ============================================================
 * hud_test.cpp — Standalone HUD preview test
 * ------------------------------------------------------------
 * Build:   make test-hud
 * Run:     ./test-hud
 * Purpose: Verify that the HUD (health bars, round timer, winner
 *          messages) renders correctly, using the SAME HUD, Font,
 *          Renderer and Character classes the game uses.
 *
 * Controls while running:
 *   1 ............ damage Player 1 (health bar drops)
 *   2 ............ damage Player 2 (health bar drops)
 *   t ............ tick the round timer down by 1 second
 *   n ............ cycle phase: fighting -> round over -> match over
 *   ESC / q ...... quit
 * ============================================================ */

#include "GL/glut.h"

#include "entities/Character.h"
#include "entities/CharacterRoster.h"
#include "render/HUD.h"
#include "render/Font.h"
#include "render/Renderer.h"
#include "computer_graphics/core/Orthographic.h"

#include <cstdio>
#include <cstdlib>

static Character character1(480.0f, 120.0f, 1000.0f);
static Character character2(1440.0f, 120.0f, 1000.0f);
static HUD hud1(character1, 200.0f);
static HUD hud2(character2, 200.0f);
static Font gameFont;

static int roundSeconds = 90;
// 0 = fighting, 1 = round over, 2 = match over
static int phase = 0;

// ------------------------------------------------------------------
// Render one frame — exact same HUD calls as Game::render()
// ------------------------------------------------------------------
static void display(void)
{
    Renderer::clear(0.10f, 0.10f, 0.15f, 1.0f);

    // Orthographic HUD space: 1920 x 1080, origin bottom-left
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Custom orthographic projection from the cg math core
    glLoadMatrixf(cg::Orthographic(0.0f, 1920.0f, 0.0f, 1080.0f).toMatrix().data());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Reference strips so you can confirm quads render in this space
    Renderer::drawQuad(0, 1040, 1920, 40, 0.25f, 0.25f, 0.30f); // top edge

    // --- Health bars (same positions as the game) ---
    hud1.drawHealthBar(50.0f, 1000.0f, 30.0f, false);
    hud2.drawHealthBar(1670.0f, 1000.0f, 30.0f, true);

    // --- Round timer (font text, top center) ---
    hud1.drawTimer(gameFont, roundSeconds, 50.0f);

    // --- Phase messages (font text, screen center) ---
    if (phase == 1)
        hud1.drawWinnerMessage(gameFont, "Player 1 Wins the Round!", "Round Over");
    else if (phase == 2)
        hud2.drawWinnerMessage(gameFont, "Player 2 is Champion!", "Match Over - R rematch, Q quit");

    glutSwapBuffers();
}

// ------------------------------------------------------------------
// Periodic update callback (matches game main loop style)
// ------------------------------------------------------------------
static void update(int)
{
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// ------------------------------------------------------------------
// Keyboard shortcuts
// ------------------------------------------------------------------
static void keyboard(unsigned char key, [[maybe_unused]]int x, [[maybe_unused]]int y)
{
    switch (key)
    {
        case '1': character1.takeDamage(150.0f); break;
        case '2': character2.takeDamage(150.0f); break;
        case 't': if (roundSeconds > 0) roundSeconds--; break;
        case 'r': roundSeconds = 90;
                  character1.resetForRound(480.0f, 120.0f);
                  character2.resetForRound(1440.0f, 120.0f); break;
        case 'n': phase = (phase + 1) % 3; break;
        case 27:  // ESC
        case 'q': std::exit(0); break;
        default:  break;
    }
}

// ------------------------------------------------------------------
// Entry point
// ------------------------------------------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("HUD Test - 1920x1080 virtual space");

    Renderer::init();

    // Load the same roster data the game uses
    CharacterData data = CharacterRoster::getCharacter("Stickman");
    character1.applyData(data);
    character2.applyData(data);

    bool fontOK = gameFont.load("assets/fonts/main.ttf", 48.0f);
    hud1.setViewportSize(1920.0f, 1080.0f);
    hud2.setViewportSize(1920.0f, 1080.0f);

    // Map 1280x720 window onto the 1920x1080 virtual canvas (2/3 scale)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Custom orthographic projection from the cg math core
    glLoadMatrixf(cg::Orthographic(0.0f, 1920.0f, 0.0f, 1080.0f).toMatrix().data());
    glMatrixMode(GL_MODELVIEW);

    printf("===========================================\n");
    printf(" HUD TEST\n");
    printf("-------------------------------------------\n");
    printf(" font loaded : %s\n", fontOK ? "YES" : "NO");
    printf(" font size   : %.1f\n", gameFont.getTextHeight());
    printf(" '90' width  : %.1f px (0 means font failed)\n", gameFont.getTextWidth("90"));
    printf(" P1 health   : %.0f / %.0f\n", character1.getHealth(), character1.getMaxHealth());
    printf(" P2 health   : %.0f / %.0f\n", character2.getHealth(), character2.getMaxHealth());
    printf("-------------------------------------------\n");
    printf(" Keys: 1/2 damage, t timer--, n cycle phase,\n");
    printf("       r reset, q quit\n");
    printf("===========================================\n");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}

#endif // HUD_TEST_H