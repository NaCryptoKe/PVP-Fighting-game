/*
 * Load the image texture to the GPU
 * TextureData is used to get the texture's info
 */
#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "GL/glut.h"

struct TextureData
{
    GLuint id;
    int width;      // Width of the image
    int height;     // Height of the image
};

// Function declaration
TextureData loadTexture(const char* filePath);

#endif // TEXTURE_HPP