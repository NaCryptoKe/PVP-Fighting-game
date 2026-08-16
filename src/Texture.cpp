#include "src/Texture.hpp"

#include <stdio.h>

// Define implementation ONCE before including stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

TextureData loadTexture(const char* filePath)
{
    int width, height, channels;
    // Channels are what colors they have
    // 1 channel Grayscale
    // 2 channels grayscale + alpha
    // 3 channels RGB
    // 4 channels RGBA

    /*
     * Because image format store data from top-left
     * And OpenGL starts from bottom-left
     * We flip the image at the CPU before reaching the GPU
    */
    // 1. Fix the coordinate system so row 0 aligns with OpenGL's bottom-left standard
    stbi_set_flip_vertically_on_load(true);
    
    // 2. CPU RAM Allocation: Read file from disk, decode compressed PNG bytes,
    // Force 4 channels (RGBA) for alpha transparency
    unsigned char* data = stbi_load(filePath, &width, &height, &channels, 4);

    // 3. Error handling: If file is missing or corrupted, exit safely.
    if (!data)
    {
        printf("Failed to load texture: %s\n", filePath);
        return { 0, 0, 0 };
    }

    // 4. GPU Container Generation: Ask OpenGL for a locker key (ID).
    // OpenGL's representation of uint/unsigned int
    GLuint textureID;   // An ID that points to the texture, doesn't hold it
    glGenTextures(1, &textureID);   // Generate a texture.
                                    // Parameter 1: amount of texture
                                    // Parameter 2: giving the adress back
    
    // 5. State Machine Activation: Make this new texture ID the active target.
    glBindTexture(GL_TEXTURE_2D, textureID);    // Make the texture active

    // 6. GPU Configuration: Set scaling to NEAREST (crisp pixels) and WRAP to REPEAT.
    // Using Nearest-Neighbour interpolation. Make them sharp and blocky
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 7. The Bridge (CPU to GPU): Copy raw byte array across the bus from CPU RAM 
    // into GPU VRAM under the active texture ID.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // 8. Free the image memory on the CPU side
    stbi_image_free(data);

    // 9. Return the package: Give the caller the GPU ID and dimensions wrapped in a struct.
    return { textureID, width, height };
}