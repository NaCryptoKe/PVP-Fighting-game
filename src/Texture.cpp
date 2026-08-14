#include "src/Texture.hpp"

#include <stdio.h>

// Define implementation ONCE before including stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

GLuint loadTexture(const char* filePath)
{
    int width, height, channels;

    stbi_set_flip_vertically_on_load(true);

    // Force 4 channels (RGBA) for alpha transparency
    unsigned char* data = stbi_load(filePath, &width, &height, &channels, 4);

    if (!data)
    {
        printf("Failed to load texture: %s\n", filePath);
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Fixed typo: glTexParameteri (not glTextParameteri)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Texture wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Free the image memory on the CPU side
    stbi_image_free(data);

    return textureID;
}