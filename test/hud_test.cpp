#include "GL/glut.h"
#include <stdio.h>

#include "src/HUD.hpp"
#include "src/Character.hpp"
#include "src/Renderer.hpp"

Character Player;
HUD hud(Player, 450.0f);
Font font;
char character[] = "Noob Saibot";

void display()
{
    Renderer::clear(0.08f, 0.08f, 0.12f);
    glLoadIdentity();
    hud.drawHealthBar(50.0f, 50.0f, 50.0f, false);
    hud.drawWinnerMessage(font, character);
    hud.drawTimer(font, 56, 200.0f);

    glutSwapBuffers();
}

void update(int)
{
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void reshape(int width, int height) {
    if (height == 0) height = 1;

    const float targetAspect = 1920.0f / 1080.0f;
    float windowAspect = (float)width / (float)height;

    int vpX = 0, vpY = 0;
    int vpWidth = width, vpHeight = height;

    if (windowAspect > targetAspect) {
        vpWidth = static_cast<int>(height * targetAspect);
        vpX = (width - vpWidth) / 2;
    } else {
        vpHeight = static_cast<int>(width / targetAspect);
        vpY = (height - vpHeight) / 2;
    }

    hud.setViewportSize(
        static_cast<float>(width),
        static_cast<float>(height)
    );

    glViewport(vpX, vpY, vpWidth, vpHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1920.0, 0.0, 1080.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGBA
    );

    glutInitWindowSize(800, 600);
    glutCreateWindow("HUD Test");

    if (!font.load("assets/fonts/mainFont.ttf", 32.0f)) 
        printf("Font loading failed!\n");

    Player.applyHit(85, false);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);

    glutMainLoop();

    return 0;
}