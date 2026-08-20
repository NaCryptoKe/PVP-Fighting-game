#include <iostream>
#include <GL/glut.h>

#include "src/Game.hpp"
#include "src/Input.hpp"

#define MAJOR 0
#define MINOR 3

Game game;

void display()
{
    game.render();
}

void update(int)
{
    game.update();
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void reshape(int width, int height)
{
    game.reshape(width, height);
}

int main(int argc, char** argv)
{
    const char* str = "2D Fighting Game V";
    char title[128];
    snprintf(title, sizeof(title), "%s%d.%d", str, MAJOR, MINOR);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(title);
    //glutFullScreen();

    game.init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeyDown);
    glutKeyboardUpFunc(handleKeyUp);
    glutSpecialFunc(handleSpecialKeyDown);   // arrow keys for Player 2
    glutSpecialUpFunc(handleSpecialKeyUp);
    glutTimerFunc(0, update, 0);

    glutMainLoop();

    return 0;
}