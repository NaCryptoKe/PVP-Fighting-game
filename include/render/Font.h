#ifndef FONT_H
#define FONT_H

#include "GL/glut.h"

struct Font {
    GLuint textureID;
    float fontSize;

    bool load(const char* filepath, float size);
    void renderText(const char* text, float x, float y, float r = 1.0f, float g = 1.0f, float b = 1.0f);
    float getTextWidth(const char* text) const;
    float getTextHeight() const;
};

#endif // FONT_H
