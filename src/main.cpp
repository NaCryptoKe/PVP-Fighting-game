#include <GL/glut.h>
#include <stdio.h>

#define MAJOR 0
#define MINOR 1

void display()
{
    // clear the screen buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Double buffering
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    const char* str = "PvP Game V";
    char title[128];
    snprintf(title, sizeof(title), "%s%d.%d", str, MAJOR, MINOR);

    // Initialize GLUT
    glutInit(&argc, argv);

    // GLUT_DOUBLE enables smooth double buffeing (front + back buffer)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);   // Initial fallback screen
    glutInitWindowPosition(100, 100);
    glutCreateWindow(title);
    glutFullScreen();           // Going into fullscreen mode

    // Register GLUT event callbacks
    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}

