#include "GL/glut.h"
#include <iostream>

#include "src/Renderer.hpp"
#include "src/Texture.hpp"

TextureData texture;

void display()
{
    // ----------------------------------------
    // 1. Clear the screen
    // ----------------------------------------
    Renderer::clear(0.08f, 0.08f, 0.12f);

    // ----------------------------------------
    // 2. Test drawQuad()
    // ----------------------------------------

    // Red quad
    Renderer::drawQuad(
        100.0f, 100.0f,
        200.0f, 100.0f,
        1.0f, 0.0f, 0.0f
    );

    // Green quad
    Renderer::drawQuad(
        350.0f, 100.0f,
        200.0f, 100.0f,
        0.0f, 1.0f, 0.0f
    );

    // Blue quad
    Renderer::drawQuad(
        600.0f, 100.0f,
        100.0f, 200.0f,
        0.0f, 0.0f, 1.0f
    );

    // ----------------------------------------
    // 3. Test drawSprite()
    // ----------------------------------------

    // Normal sprite
    Renderer::drawSprite(
        texture.id,
        100.0f, 350.0f,
        150.0f, 150.0f,
        false
    );

    // Horizontally flipped sprite
    Renderer::drawSprite(
        texture.id,
        300.0f, 350.0f,
        150.0f, 150.0f,
        true
    );

    // ----------------------------------------
    // 4. Test drawFighterSprite()
    // ----------------------------------------

    // Normal fighter
    Renderer::drawFighterSprite(
        texture,
        550.0f,   // footX
        350.0f,   // footY
        0.5f,     // scale
        false     // flipX
    );

    // Flipped fighter
    Renderer::drawFighterSprite(
        texture,
        700.0f,   // footX
        350.0f,   // footY
        0.5f,     // scale
        true      // flipX
    );

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Use a 1920x1080 virtual coordinate system
    glOrtho(
        0.0,
        800.0,
        0.0,
        600.0,
        -1.0,
        1.0
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
    glutCreateWindow("Renderer Test");

    // ----------------------------------------
    // 2. Initialize Renderer
    // ----------------------------------------
    Renderer::init();

    // ----------------------------------------
    // 3. Load test texture
    // ----------------------------------------
    texture = loadTexture("assets/characters/player.png");

    if (texture.id == 0)
    {
        std::cerr << "Failed to load texture!\n";
        return 1;
    }

    std::cout << "Texture loaded successfully!\n";
    std::cout << "Texture ID: " << texture.id << '\n';
    std::cout << "Width: " << texture.width << '\n';
    std::cout << "Height: " << texture.height << '\n';

    // ----------------------------------------
    // 4. Register callbacks
    // ----------------------------------------
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // ----------------------------------------
    // 5. Start rendering
    // ----------------------------------------
    glutMainLoop();

    return 0;
}