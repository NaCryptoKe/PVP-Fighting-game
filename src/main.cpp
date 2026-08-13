#include <GL/glew.h>    // for accessing the GPU
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

    int currentTime = glutGet(GLUT_ELAPSED_TIME); // Amount of total time in milliseconds
    int timeInterval = currentTime - previousTime; // Change in time

    if (timeInterval > 1000)        // if it exceeds 1 second
    {
        float fps = frameCount / (timeInterval / 1000.0f);  // frame per second
        previousTime = currentTime;
        frameCount = 0;

        /*
         * Example:
         * lets assume the frame Count is 62, and the time Interval was 1033ms
         * First we convert to second: 1033ms / 1000.0f = 1.033 seconds
         * Calculate FPS: 62 frames / 1.033 secons = 60.019 FPS
         */

        snprintf(fpsString, sizeof(fpsString), "FPS: %.1f", fps);
    }
}

void display()
{
    // clear the screen buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);
    renderBitmapString(-0.9f, 0.85f, GLUT_BITMAP_HELVETICA_18, fpsString);

    // Double buffering
    glutSwapBuffers();
}

void timer(int)
{
    calculateFPS();

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void update()
{
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
    //glutFullScreen();           // Going into fullscreen mode

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "GLEW Initialization Failed: %s\n", glewGetErrorString(err));
        return 1;
    }

    // Register GLUT event callbacks
    glutDisplayFunc(display);

    // Recurring time loop
    glutTimerFunc(0, timer, 0);

    glutMainLoop();

    return 0;
}

