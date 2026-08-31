#include "GL/glut.h"
#include <cstdio>

void display()
{
    glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    printf("Step 1: calling glutInit...\n");
    fflush(stdout);
    glutInit(&argc, argv);

    printf("Step 2: setting display mode...\n");
    fflush(stdout);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    printf("Step 3: creating window...\n");
    fflush(stdout);
    glutInitWindowSize(400, 300);
    glutCreateWindow("GLUT Minimal Test");

    printf("Step 4: window created, registering display func...\n");
    fflush(stdout);
    glutDisplayFunc(display);

    printf("Step 5: entering main loop (close the window to exit)...\n");
    fflush(stdout);
    glutMainLoop();

    return 0;
}