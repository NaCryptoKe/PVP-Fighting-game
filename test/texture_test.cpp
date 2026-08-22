#include "GL/glut.h"
#include <stdio.h>

#include "src/Texture.hpp"

int main(int argc, char** argv)
{
    // 1. Initialize GLUT
    glutInit(&argc, argv);

    // 2. Create an OpenGL context/window
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Texture Test");

    // 3. Now OpenGL functions are safe to call
    TextureData texture_1 = loadTexture("assets/characters/player.png");
    TextureData texture_2 = loadTexture("assets/characters/player.png");

    // 4. Check whether loading succeeded
    if (texture_1.id == 0)
    {
        printf("Texture loading failed!\n");
        return 1;
    }
    if (texture_2.id == 0)
    {
        printf("Texture loading failed!\n");
        return 1;
    }

    printf("Texture 1 loaded successfully!\n");
    printf("Texture ID: %u\n", texture_1.id);
    printf("Width: %d\n", texture_1.width);
    printf("Height: %d\n", texture_1.height);

    printf("Texture 2 loaded successfully!\n");
    printf("Texture ID: %u\n", texture_2.id);
    printf("Width: %d\n", texture_2.width);
    printf("Height: %d\n", texture_2.height);

    return 0;
}