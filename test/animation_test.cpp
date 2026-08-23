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

void printAnimationState(const char* label, Animation& anim)
{
    printf("%s\n", label);

    printf("Frame index: %d\n",
           anim.getCurrentFrameIndex());

    printf("Frame count: %d\n",
           anim.getFrameCount());

    printf("Finished: %s\n",
           anim.isFinished() ? "YES" : "NO");

    printf("Texture ID: %u\n\n",
           anim.getCurrentTexture().id);
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

    // ============================================================
    // 1. Test constructor
    // ============================================================

    Animation anim;

    printAnimationState(
        "=== Initial Animation State ===",
        anim
    );


    // ============================================================
    // 2. Test loadFromFiles()
    // ============================================================

    bool loaded = anim.loadFromFiles(
        "assets/characters/chun-li/idle/",
        3,
        0.18f,
        false
    );

    printf("=== After Loading Animation ===\n");

    printf("Loaded: %s\n",
           loaded ? "YES" : "NO");

    printf("Frame count: %d\n",
           anim.getFrameCount());

    printf("Current frame: %d\n",
           anim.getCurrentFrameIndex());

    printf("Finished: %s\n",
           anim.isFinished() ? "YES" : "NO");

    printf("Current texture ID: %u\n\n",
           anim.getCurrentTexture().id);


    // ============================================================
    // 3. Test update() before frame duration
    // ============================================================

    printf("=== Testing update() ===\n");

    anim.update(0.05f);

    printf("After 0.05 seconds:\n");
    printf("Frame: %d\n",
           anim.getCurrentFrameIndex());

    printf("Finished: %s\n\n",
           anim.isFinished() ? "YES" : "NO");


    // ============================================================
    // 4. Continue accumulating time
    // ============================================================

    anim.update(0.05f);

    printf("After 0.10 seconds:\n");
    printf("Frame: %d\n",
           anim.getCurrentFrameIndex());

    printf("Finished: %s\n\n",
           anim.isFinished() ? "YES" : "NO");


    anim.update(0.05f);

    printf("After 0.15 seconds:\n");
    printf("Frame: %d\n",
           anim.getCurrentFrameIndex());

    printf("Finished: %s\n\n",
           anim.isFinished() ? "YES" : "NO");


    // 0.15 + 0.05 = 0.20
    // Frame duration = 0.18
    // Therefore frame 0 -> frame 1

    anim.update(0.05f);

    printf("After 0.20 seconds:\n");
    printf("Frame: %d\n",
           anim.getCurrentFrameIndex());

    printf("Finished: %s\n\n",
           anim.isFinished() ? "YES" : "NO");


    // ============================================================
    // 5. Reach the final frame
    // ============================================================

    // Current accumulated time is approximately 0.02.
    //
    // Add 0.16:
    //
    // 0.02 + 0.16 = 0.18
    //
    // This moves frame 1 -> frame 2.

    anim.update(0.16f);

    printf("After reaching frame 2:\n");
    printf("Frame: %d\n",
           anim.getCurrentFrameIndex());

    printf("Finished: %s\n\n",
           anim.isFinished() ? "YES" : "NO");


    // ============================================================
    // 6. Finish the non-looping animation
    // ============================================================

    // Another 0.18 seconds should attempt to move
    // frame 2 -> frame 3.
    //
    // But frame 3 doesn't exist.
    //
    // Since looping = false:
    //
    // currentFrame = last frame
    // finished = true

    anim.update(0.18f);

    printf("After reaching the end:\n");
    printf("Frame: %d\n",
           anim.getCurrentFrameIndex());

    printf("Finished: %s\n\n",
           anim.isFinished() ? "YES" : "NO");


    // ============================================================
    // 7. Test update() after finished
    // ============================================================

    // update() should do nothing because finished == true.

    anim.update(1.0f);

    printf("After update(1.0) on finished animation:\n");
    printf("Frame: %d\n",
           anim.getCurrentFrameIndex());

    printf("Finished: %s\n\n",
           anim.isFinished() ? "YES" : "NO");


    // ============================================================
    // 8. Test reset()
    // ============================================================

    anim.reset();

    printf("=== After Reset ===\n");

    printf("Frame: %d\n",
           anim.getCurrentFrameIndex());

    printf("Finished: %s\n",
           anim.isFinished() ? "YES" : "NO");

    printf("Frame count: %d\n\n",
           anim.getFrameCount());


    // ============================================================
    // 9. Test looping animation
    // ============================================================

    printf("=== Testing Looping Animation ===\n");

    anim.setLooping(true);

    // The animation is currently at frame 0 after reset.

    // Each frame lasts 0.18 seconds.
    //
    // 0.18 -> frame 1
    // 0.36 -> frame 2
    // 0.54 -> frame 0 (loops)

    anim.update(0.18f);

    printf("After 0.18 seconds:\n");
    printf("Frame: %d\n",
           anim.getCurrentFrameIndex());

    printf("Finished: %s\n\n",
           anim.isFinished() ? "YES" : "NO");


    anim.update(0.18f);

    printf("After 0.36 seconds:\n");
    printf("Frame: %d\n",
           anim.getCurrentFrameIndex());

    printf("Finished: %s\n\n",
           anim.isFinished() ? "YES" : "NO");


    anim.update(0.18f);

    printf("After 0.54 seconds:\n");
    printf("Frame: %d\n",
           anim.getCurrentFrameIndex());

    printf("Finished: %s\n\n",
           anim.isFinished() ? "YES" : "NO");


    // ============================================================
    // 10. Test multiple frames being skipped in one update
    // ============================================================

    anim.reset();

    printf("=== Testing Large Delta Time ===\n");

    // 0.54 seconds / 0.18 seconds per frame = 3 frames.
    //
    // Starting at frame 0:
    //
    // frame 0 -> frame 1 -> frame 2 -> frame 0

    anim.update(0.54f);

    printf("After update(0.54):\n");
    printf("Frame: %d\n",
           anim.getCurrentFrameIndex());

    printf("Finished: %s\n\n",
           anim.isFinished() ? "YES" : "NO");


    // ============================================================
    // 11. Register GLUT callbacks
    // ============================================================

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}