#include "GL/glut.h"
#include <iostream>

#include "src/Sprite.hpp"
#include "src/Renderer.hpp"
#include "src/Texture.hpp"

Sprite sprite1;
Sprite sprite2;

void display()
{
    Renderer::clear(0.08f, 0.08f, 0.12f);

    // ----------------------------------------
    // Draw Sprite 1
    // ----------------------------------------
    sprite1.draw();

    // ----------------------------------------
    // Draw Sprite 2
    // ----------------------------------------
    sprite2.draw();

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

int main(int argc, char** argv)
{
    // ----------------------------------------
    // 1. Create OpenGL context
    // ----------------------------------------
    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGBA
    );

    glutInitWindowSize(800, 600);
    glutCreateWindow("Sprite Test");

    // ----------------------------------------
    // 2. Initialize renderer
    // ----------------------------------------
    Renderer::init();

    // ----------------------------------------
    // 3. Load texture
    // ----------------------------------------
    TextureData texture = loadTexture("assets/characters/player.png");

    if (texture.id == 0)
    {
        std::cerr << "Failed to load texture!\n";
        return 1;
    }

    std::cout << "Texture loaded successfully!\n";
    std::cout << "ID: " << texture.id << '\n';
    std::cout << "Width: " << texture.width << '\n';
    std::cout << "Height: " << texture.height << '\n';

    // ----------------------------------------
    // 4. Test default constructor
    // ----------------------------------------
    // Assign texture using setTexture()
    sprite1.setTexture(texture);

    // Set foot anchor position
    sprite1.setPosition(250.0f, 100.0f);

    // Set scale
    sprite1.setScale(1.5f);

    // Don't flip
    sprite1.setFlip(false);

    // ----------------------------------------
    // 5. Test TextureData constructor
    // ----------------------------------------
    Sprite spriteWithTexture(texture);

    spriteWithTexture.setPosition(550.0f, 100.0f);
    spriteWithTexture.setScale(2.5f);
    spriteWithTexture.setFlip(true);

    // We want this object displayed by the callback.
    sprite2 = spriteWithTexture;

    // ----------------------------------------
    // 6. Register callbacks
    // ----------------------------------------
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // ----------------------------------------
    // 7. Start event loop
    // ----------------------------------------
    glutMainLoop();

    return 0;
}