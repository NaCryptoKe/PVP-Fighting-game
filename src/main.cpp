#include <GL/glew.h>    // For accessing GPU extensions
#include <GL/glut.h>
#include <stdio.h>
#include <glm/gtc/type_ptr.hpp> // Needed for passing GLM matrices to OpenGL/Shaders

#include "src/Camera.h"

#define MAJOR 0
#define MINOR 1

// Global Camera Instance (Defaults to Perspective 2.5D view)
Camera mainCamera(glm::vec3(0.0f, 2.0f, 10.0f), glm::vec3(0.0f, 2.0f, 0.0f));

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

    // =========================================================================
    // RENDER 3D SCENE
    // =========================================================================
    
    // IF USING SHADERS (Modern OpenGL Core Profile)
    // glm::mat4 view = mainCamera.GetViewMatrix();
    // glm::mat4 proj = mainCamera.GetProjectionMatrix();
    // glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view));
    // glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, glm::value_ptr(proj));

    // IF USING FIXED-FUNCTION PIPELINE (Legacy OpenGL)
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(mainCamera.GetProjectionMatrix()));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(mainCamera.GetViewMatrix()));

    // TODO: Render your 3D models here (located on the Z = 0 plane)


    // =========================================================================
    // RENDER 2D HUD OVERLAY (FPS TEXT)
    // =========================================================================
    // Push identity matrices so HUD text stays static relative to the window
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
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
    
    // Update camera aspect ratio upon window resize
    mainCamera.UpdateAspectRatio(static_cast<float>(width), static_cast<float>(height));
}

void update()
{
    // Game logic / movement updates
}

int main(int argc, char** argv)
{
    const char* str = "PvP Game V";
    char title[128];
    snprintf(title, sizeof(title), "%s%d.%d", str, MAJOR, MINOR);

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(title);
    glutFullScreen();

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "GLEW Initialization Failed: %s\n", glewGetErrorString(err));
        return 1;
    }

    // OpenGL State Settings
    glEnable(GL_DEPTH_TEST); // Enable Z-buffer depth sorting for 3D meshes

    // Register GLUT Event Callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // Recurring Timer Loop
    glutTimerFunc(0, timer, 0);

    // Enter Main GLUT Loop
    glutMainLoop();

    return 0;
}