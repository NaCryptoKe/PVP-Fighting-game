#ifndef FONT_HPP
#define FONT_HPP

#include "GL/glut.h"
#include "stb/stb_truetype.h"

struct Font {
    GLuint textureID;
    stbtt_bakedchar cdata[96]; // Character data for ASCII 32..126
    float fontSize;

    bool load(const char* filepath, float size);
    void renderText(const char* text, float x, float y, float r = 1.0f, float g = 1.0f, float b = 1.0f);
    float getTextWidth(const char* text) const;
    float getTextHeight() const;
};

#endif // FONT_HPP