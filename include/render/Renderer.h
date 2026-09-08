#ifndef RENDERER_H
#define RENDERER_H

#include "GL/glut.h"

#include "computer_graphics/core/Transform.h"
#include "graphics/Texture.h"

// All drawing is built on the cg math core: every quad/sprite is a UNIT quad
// placed by a cg::Transform (T * R * S) that gets uploaded with
// glMultMatrixf, and both projections come from cg::Orthographic.
class Renderer 
{
public:
    // Initialize OpenGL rendering states (Blending, Depth testing off)
    static void init();

    // Clear the canvas to a specific background color
    static void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

    // Draw a filled + outlined quad through a cg::Transform. The transform's
    // scale IS the on-screen size; rotation and negative scales work too.
    static void drawColoredQuad(const cg::Transform& transform, float r, float g, float b, float a = 1.0f);

    // Bottom-left anchored convenience wrapper around drawColoredQuad
    static void drawQuad(
        float x, float y, 
        float width, float height, 
        float r, float g, float b, float a = 1.0f
    );

    // Draw a textured unit quad through a cg::Transform. The transform's
    // scale is the on-screen size; a negative X scale mirrors the image.
    static void drawTexturedQuad(GLuint textureID, const cg::Transform& transform);

    // Draw a textured quad anchored at its bottom-left corner (x, y).
    // Thin wrapper that builds a cg::Transform and delegates.
    static void drawSprite(
        GLuint textureID, 
        float x, float y,
        float width, float height,
        bool flipX = false
    );

    // Draw a fighter sprite anchored at its FEET (bottom-center, on the
    // ground line) with a uniform scale multiplier and optional mirror flip.
    static void drawFighterSprite(
        TextureData texture, 
        float footX, float footY,
        float scale, bool flipX
    );
};

#endif // RENDERER_H
