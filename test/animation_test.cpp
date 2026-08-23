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
    // 3. Test update()
    // ----------------------------------------
    printf("\n=== After Update Animation ===\n");
    anim.update(0.05f);
    printf("Frame index after 0.05: %d\n",
           anim.getCurrentFrameIndex());
    printf("Is animation finished: %s\n\n",
           anim.isFinished() ? "YES" : "NO");   
    anim.update(0.05f);
    printf("Frame index after 0.10: %d\n",
           anim.getCurrentFrameIndex());
    printf("Is animation finished: %s\n\n",
           anim.isFinished() ? "YES" : "NO");       
    anim.update(0.05f);
    printf("Frame index after 0.15: %d\n",
           anim.getCurrentFrameIndex());
    printf("Is animation finished: %s\n\n",
           anim.isFinished() ? "YES" : "NO");       
    anim.update(0.05f);
    printf("Frame index after 0.20: %d\n",
           anim.getCurrentFrameIndex());
    printf("Is animation finished: %s\n\n",
           anim.isFinished() ? "YES" : "NO");
           
    // For loop till end of the animation
    for (float i = 0; i < 0.36f; i += 0.05f)
    {
       anim.update(0.05f);
       printf("Frame index after %.2f: %d\n", i + 0.20f,
           anim.getCurrentFrameIndex());
       printf("Is animation finished: %s\n\n",
           anim.isFinished() ? "YES" : "NO");
    }

    // ----------------------------------------
    // 4. Animation that loops
    // ----------------------------------------
    printf("\n=== After Looping Animation ===\n"); 
    anim.setLooping(true);
    anim.reset();
    for (float i = 0; i < 1.0f; i += 0.05f)
    {
       anim.update(0.05f);
       printf("Frame index after %.2f: %d\n", i + 0.20f,
           anim.getCurrentFrameIndex());
       printf("Is animation finished: %s\n\n",
           anim.isFinished() ? "YES" : "NO");
    }

    // ----------------------------------------
    // 5. Register callbacks
    // ----------------------------------------
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // ----------------------------------------
    // 6. Start event loop
    // ----------------------------------------
    glutMainLoop();

    return 0;
}