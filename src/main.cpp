#include <iostream>
#include <GL/glut.h>

#include "src/Game.hpp"

#define MAJOR 0
#define MINOR 2

Game game;

void display()
{
    game.render();
}

void update(int)
{
    game.update();
    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Target ~60 FPS
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

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(title);
    glutFullScreen();

    game.init();

    // Register GLUT Event Callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);

    // Enter Main GLUT Loop
    glutMainLoop();

    return 0;
}