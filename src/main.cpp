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

void mouseButtonCallback(int button, int state, int x, int y)
{
    game.onMouseButton(button, state, x, y);
}

void mouseMoveCallback(int x, int y)
{
    game.onMouseMove(x, y);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    glutInitWindowSize(1280, 720);   // 16:9 — maps 1:1 onto the 1920x1080 HUD space
    glutCreateWindow("PVP Fighting Game");

    game.init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyboardDownCallback);
    glutKeyboardUpFunc(keyboardUpCallback);
    glutSpecialFunc(specialKeyDownCallback);
    glutSpecialUpFunc(specialKeyUpCallback);

    // Mouse: clickable end-of-match buttons + hover highlight
    glutMouseFunc(mouseButtonCallback);
    glutPassiveMotionFunc(mouseMoveCallback);

    glutTimerFunc(0, update, 0);    // Opted for glutTimerFunc instead of glutIdleFunc to avoid unnecessary CPU usage

    glutMainLoop();
    return 0;
}
