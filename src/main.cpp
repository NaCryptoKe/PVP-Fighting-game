#include <iostream>
#include <GL/glut.h>
#include <stdio.h>

#define MAJOR 0
#define MINOR 1

char fpsString[32] = "FPS: 0.0";
int frameCount = 0;
int previousTime = 0;

void renderBitmapString(float x, float y, void *font, const char *string)
{
    glRasterPos2f(x, y);
    for (const char *c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void calculateFPS()
{
    frameCount++;

    int currentTime = glutGet(GLUT_ELAPSED_TIME); // Total time in milliseconds
    int timeInterval = currentTime - previousTime; // Change in time

    if (timeInterval > 1000) // Exceeds 1 second
    {
        float fps = frameCount / (timeInterval / 1000.0f);
        previousTime = currentTime;
        frameCount = 0;

        snprintf(fpsString, sizeof(fpsString), "FPS: %.1f", fps);
    }
}

void display()
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // Render screen-space FPS counter
    glColor3f(1.0f, 1.0f, 1.0f);
    renderBitmapString(-0.95f, 0.85f, GLUT_BITMAP_TIMES_ROMAN_24, fpsString);

    // Restore 3D matrices
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // Swap front and back buffers
    glutSwapBuffers();
}

void timer(int)
{
    calculateFPS();

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // Target ~60 FPS
}

void reshape(int width, int height) {
    if (height == 0) height = 1; // Prevent division by zero
    glViewport(0, 0, width, height);
}

void update()
{
    // Game logic / movement updates
}

int main(int argc, char** argv)
{
    const char* str = "2D Fighting Game V";
    char title[128];
    snprintf(title, sizeof(title), "%s%d.%d", str, MAJOR, MINOR);

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(title);
    glutFullScreen();
    
    // Register GLUT Event Callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // Recurring Timer Loop
    glutTimerFunc(0, timer, 0);

    // Enter Main GLUT Loop
    glutMainLoop();

    return 0;
}