#include "GL/glut.h"
#include <iostream>

#include "src/Font.hpp"

Font font;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Render some test text
    font.renderText(
        "Hello, World!",
        100.0f,
        200.0f,
        1.0f,
        1.0f,
        1.0f
    );

    font.renderText(
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        100.0f,
        300.0f,
        1.0f,
        0.0f,
        0.0f
    );

    font.renderText(
        "abcdefghijklmnopqrstuvwxyz",
        100.0f,
        400.0f,
        0.0f,
        1.0f,
        0.0f
    );

    font.renderText(
        "0123456789 !@#$%^&*()",
        100.0f,
        500.0f,
        0.0f,
        0.5f,
        1.0f
    );

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Use pixel coordinates:
    // (0, 0) is the bottom-left
    // (800, 600) is the top-right
    glOrtho(0, 800, 0, 600, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    // ----------------------------------------
    // 1. Initialize GLUT
    // ----------------------------------------
    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGBA
    );

    glutInitWindowSize(800, 600);
    glutCreateWindow("Font Test");

    // ----------------------------------------
    // 2. Load the font
    // ----------------------------------------
    if (!font.load("assets/fonts/mainFont.ttf", 32.0f))
    {
        std::cerr << "Failed to load font!\n";
        return 1;
    }

    std::cout << "Font loaded successfully!\n";
    std::cout << "Texture ID: " << font.textureID << '\n';
    std::cout << "Font size: " << font.fontSize << '\n';

    // ----------------------------------------
    // 3. Register callbacks
    // ----------------------------------------
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // ----------------------------------------
    // 4. Start the GLUT event loop
    // ----------------------------------------
    glutMainLoop();

    return 0;
}