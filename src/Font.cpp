#include "src/Font.hpp"
#include <stdio.h>
#include <vector>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

bool Font::load(const char* filepath, float size)
{
    fontSize = size;

    // --------------------------------------------------------
    // Read TTF file into memory
    // --------------------------------------------------------

    FILE* file = fopen(filepath, "rb");

    if (!file)
    {
        printf("Failed to open font file: %s\n", filepath);
        return false;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    std::vector<unsigned char> ttfBuffer(length);

    fread(
        ttfBuffer.data(),
        1,
        length,
        file
    );

    fclose(file);


    // --------------------------------------------------------
    // Create font atlas
    // --------------------------------------------------------

    const int atlasWidth = 512;
    const int atlasHeight = 512;

    std::vector<unsigned char> tempBitmap(
        atlasWidth * atlasHeight
    );


    // --------------------------------------------------------
    // Bake ASCII characters 32-127
    // --------------------------------------------------------

    stbtt_BakeFontBitmap(
        ttfBuffer.data(),
        0,
        fontSize,
        tempBitmap.data(),
        atlasWidth,
        atlasHeight,
        32,
        96,
        cdata
    );


    // --------------------------------------------------------
    // Create OpenGL texture
    // --------------------------------------------------------

    glGenTextures(1, &textureID);

    glBindTexture(
        GL_TEXTURE_2D,
        textureID
    );

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_ALPHA,
        atlasWidth,
        atlasHeight,
        0,
        GL_ALPHA,
        GL_UNSIGNED_BYTE,
        tempBitmap.data()
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );

    return true;
}


// ============================================================
// Render Text
// ============================================================

void Font::renderText(
    const char* text,
    float x,
    float y,
    float r,
    float g,
    float b
)
{
    glEnable(GL_TEXTURE_2D);

    glBindTexture(
        GL_TEXTURE_2D,
        textureID
    );

    glEnable(GL_BLEND);

    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );

    glColor3f(r, g, b);

    glBegin(GL_QUADS);

    while (*text)
    {
        unsigned char c =
            static_cast<unsigned char>(*text);

        if (c >= 32 && c < 128)
        {
            stbtt_aligned_quad q;

            stbtt_GetBakedQuad(
                cdata,
                512,
                512,
                c - 32,
                &x,
                &y,
                &q,
                1
            );

            float y0 =
                y - (q.y1 - y);

            float y1 =
                y - (q.y0 - y);

            glTexCoord2f(q.s0, q.t1);
            glVertex2f(q.x0, y0);

            glTexCoord2f(q.s1, q.t1);
            glVertex2f(q.x1, y0);

            glTexCoord2f(q.s1, q.t0);
            glVertex2f(q.x1, y1);

            glTexCoord2f(q.s0, q.t0);
            glVertex2f(q.x0, y1);
        }

        ++text;
    }

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}


// ============================================================
// Get Text Width
// ============================================================

float Font::getTextWidth(const char* text) const
{
    float width = 0.0f;

    while (*text)
    {
        unsigned char c =
            static_cast<unsigned char>(*text);

        if (c >= 32 && c < 128)
        {
            width += cdata[c - 32].xadvance;
        }

        ++text;
    }

    return width;
}


// ============================================================
// Get Text Height
// ============================================================

float Font::getTextHeight() const
{
    return fontSize;
}