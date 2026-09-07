#include "GL/glut.h"

#include "core/Game.h"

Game game;

void display()
{
    game.render();
}

void reshape(int width, int height)
{
    game.reshape(width, height);
}

void update(int)
{
    game.update();

    glutPostRedisplay(); // Request a redraw
    glutTimerFunc(16, update, 0); // Call update again in ~16ms or 60FPS
}

void keyboardDownCallback(unsigned char key, int x, int y)
{
    game.keyboardDownCallback(key, x, y);
}

void keyboardUpCallback(unsigned char key, int x, int y)
{
    game.keyboardUpCallback(key, x, y);
}

void specialKeyDownCallback(int key, int x, int y)
{
    game.specialKeyDownCallback(key, x, y);
}

void specialKeyUpCallback(int key, int x, int y)
{
    game.specialKeyUpCallback(key, x, y);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 800);
    glutCreateWindow("Stickman Kombat");

    game.init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyboardDownCallback);
    glutKeyboardUpFunc(keyboardUpCallback);
    glutSpecialFunc(specialKeyDownCallback);
    glutSpecialUpFunc(specialKeyUpCallback);

    glutTimerFunc(0, update, 0);    // Opted for glutTimerFunc instead of glutIdleFunc to avoid unnecessary CPU usage

    glutMainLoop();
    return 0;
}