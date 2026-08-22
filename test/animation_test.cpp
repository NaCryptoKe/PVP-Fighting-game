#include "GL/glut.h"
#include <stdio.h>

#include "src/Animation.hpp"

void display()
{
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(
        0.0f, 800.0f,
        0.0f, 600.0f,
        -1.0f, 1.0f
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGBA
    );

    glutInitWindowSize(800, 600);
    glutCreateWindow("Animation Test");

    // ----------------------------------------
    // 1. Test constructor
    // ----------------------------------------
    Animation anim;

    printf("=== Initial Animation State ===\n");
    printf("Current frame index: %d\n",
           anim.getCurrentFrameIndex());

    printf("Frame count: %d\n",
           anim.getFrameCount());

    printf("Is animation finished: %s\n",
           anim.isFinished() ? "YES" : "NO");

    printf("Current texture ID: %u\n",
           anim.getCurrentTexture().id);

    // ----------------------------------------
    // 2. Test loadFromFiles()
    // ----------------------------------------
    bool loadBool = anim.loadFromFiles(
        "assets/characters/chun-li/idle/",
        3,
        0.18f,
        false
    );

    printf("\n=== After Loading Animation ===\n");

    printf("Loaded animation: %s\n",
           loadBool ? "YES" : "NO");

    printf("Frame count: %d\n",
           anim.getFrameCount());

    printf("Current frame index: %d\n",
           anim.getCurrentFrameIndex());

    printf("Is animation finished: %s\n",
           anim.isFinished() ? "YES" : "NO");

    printf("Current texture ID: %u\n",
           anim.getCurrentTexture().id);

    // ----------------------------------------
    // 3. Register callbacks
    // ----------------------------------------
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // ----------------------------------------
    // 4. Start event loop
    // ----------------------------------------
    glutMainLoop();

    return 0;
}