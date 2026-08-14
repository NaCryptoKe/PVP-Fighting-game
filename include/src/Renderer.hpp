#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "GL/glut.h"

class Renderer {
private:
    float baseWidth  = 180.0f;
    float baseHeight  = 270.0f;

    float scale = 1.5f;

    float renderWidth = baseWidth * scale;
    float renderHeight = baseHeight * scale;
public:
    // Initialize OpenGL rendering states (Blending, Depth testing off)
    static void init();

    // Clear the canvas to a specific background color
    static void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

    // Draw a solid-color quad (Ideal for backgrounds, platforms, and hitboxes)
    static void drawQuad(float x, float y, float width, float height, float r, float g, float b, float a = 1.0f);

    // Draw a textured sprite with optional horizontal flip (Essential for Player 2 / facing left)
    static void drawSprite(GLuint textureID, float x, float y, float width, float height, bool flipX = false);
    static void drawFighterSprite(GLuint textureID, float footX, float footY, 
                                 float baseWidth, float baseHeight, 
                                 float scale, bool flipX);
};

#endif // RENDERER_HPP