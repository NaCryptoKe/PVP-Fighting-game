#include "render/Font.h"
#include <stdio.h>
#include <vector>
#include <cstring>

bool Font::load(const char* filepath, float size)
{
    fontSize = size;
    textureID = 0;  // Not using texture for GLUT font rendering
    return true;
}

void Font::renderText(
    const char* text,
    float x,
    float y,
    float r,
    float g,
    float b
)
{
    if (!text) return;

    glDisable(GL_TEXTURE_2D);
    glColor3f(r, g, b);

    // Position for text rendering
    glRasterPos2f(x, y);

    // Use GLUT's built-in bitmap font for rendering
    void *font = GLUT_BITMAP_HELVETICA_18;
    
    if (fontSize < 20.0f)
        font = GLUT_BITMAP_HELVETICA_10;
    else if (fontSize < 30.0f)
        font = GLUT_BITMAP_HELVETICA_18;
    else
        font = GLUT_BITMAP_TIMES_ROMAN_24;

    // Render each character
    for (const char *c = text; *c != '\0'; ++c)
    {
        glutBitmapCharacter(font, *c);
    }

    glEnable(GL_TEXTURE_2D);
}

float Font::getTextWidth(const char* text) const
{
    if (!text) return 0.0f;
    
    float width = 0.0f;
    void *font = GLUT_BITMAP_HELVETICA_18;
    
    if (fontSize < 20.0f)
        font = GLUT_BITMAP_HELVETICA_10;
    else if (fontSize < 30.0f)
        font = GLUT_BITMAP_HELVETICA_18;
    else
        font = GLUT_BITMAP_TIMES_ROMAN_24;

    for (const char *c = text; *c != '\0'; ++c)
    {
        width += glutBitmapWidth(font, *c);
    }

    return width;
}

float Font::getTextHeight() const
{
    return fontSize;
}

