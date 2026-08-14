#include "src/Font.hpp"
#include <stdio.h>
#include <vector>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

bool Font::load(const char* filepath, float size) {
    fontSize = size;

    // 1. Read TTF file into memory buffer
    FILE* file = fopen(filepath, "rb");
    if (!file) {
        printf("Failed to open font file: %s\n", filepath);
        return false;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    std::vector<unsigned char> ttfBuffer(length);
    fread(ttfBuffer.data(), 1, length, file);
    fclose(file);

    // 2. Create an alpha bitmap for the 512x512 font atlas
    const int atlasWidth = 512;
    const int atlasHeight = 512;
    std::vector<unsigned char> tempBitmap(atlasWidth * atlasHeight);

    // 3. Bake ASCII characters 32 to 126 into the bitmap
    stbtt_BakeFontBitmap(ttfBuffer.data(), 0, fontSize, 
                        tempBitmap.data(), atlasWidth, atlasHeight, 
                        32, 96, cdata);

    // 4. Generate OpenGL Texture from the baked bitmap
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Alpha/Luminance texture for font opacity
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, atlasWidth, atlasHeight, 
                 0, GL_ALPHA, GL_UNSIGNED_BYTE, tempBitmap.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Font::renderText(const char* text, float x, float y, float r, float g, float b) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor3f(r, g, b);

    glBegin(GL_QUADS);
    while (*text) {
        // Cast to unsigned char to avoid type-limits warning on signed 8-bit char
        unsigned char c = static_cast<unsigned char>(*text);

        if (c >= 32 && c < 128) {
            stbtt_aligned_quad q;
            
            // Pass (c - 32) instead of (*text - 32)
            stbtt_GetBakedQuad(cdata, 512, 512, c - 32, &x, &y, &q, 1);

            float y0 = y - (q.y1 - y);
            float y1 = y - (q.y0 - y);

            glTexCoord2f(q.s0, q.t1); glVertex2f(q.x0, y0);
            glTexCoord2f(q.s1, q.t1); glVertex2f(q.x1, y0);
            glTexCoord2f(q.s1, q.t0); glVertex2f(q.x1, y1);
            glTexCoord2f(q.s0, q.t0); glVertex2f(q.x0, y1);
        }
        ++text;
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}