#include "src/Renderer.hpp"

void Renderer::init() 
{
    // Enable 2D Texturing
    glEnable(GL_TEXTURE_2D);

    // Enable Alpha Blending for PNG transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Disable Depth Testing
    glDisable(GL_DEPTH_TEST);
}

void Renderer::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawQuad(float x, float y, float width, float height, float r, float g, float b, float a) 
{
    glDisable(GL_TEXTURE_2D);

    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
        glVertex2f(x, y);                  // Bottom-Left
        glVertex2f(x + width, y);          // Bottom-Right
        glVertex2f(x + width, y + height); // Top-Right
        glVertex2f(x, y + height);         // Top-Left
    glEnd();
}

void Renderer::drawSprite(GLuint textureID, float x, float y, float width, float height, bool flipX) 
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Enabling alpha blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Reset tint to full opacity white

    // Handle texture coordinate flipping for character orientation
    float uLeft  = flipX ? 1.0f : 0.0f;
    float uRight = flipX ? 0.0f : 1.0f;

    glBegin(GL_QUADS);
        // Bottom-Left
        glTexCoord2f(uLeft, 0.0f);
        glVertex2f(x, y);

        // Bottom-Right
        glTexCoord2f(uRight, 0.0f);
        glVertex2f(x + width, y);

        // Top-Right
        glTexCoord2f(uRight, 1.0f);
        glVertex2f(x + width, y + height);

        // Top-Left
        glTexCoord2f(uLeft, 1.0f);
        glVertex2f(x, y + height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void Renderer::drawFighterSprite(GLuint textureID, float footX, float footY, 
                                 float baseWidth, float baseHeight, 
                                 float scale, bool flipX) 
{
    // 1. Calculate actual rendered size based on scale multiplier
    float drawWidth  = baseWidth * scale;
    float drawHeight = baseHeight * scale;

    // 2. Adjust X position so footX is in the exact horizontal center
    float drawX = footX - (drawWidth / 2.0f);
    
    // 3. Y position stays right on the floor baseline
    float drawY = footY; 

    // 4. Pass calculated bounds to your main quad drawer
    drawSprite(textureID, drawX, drawY, drawWidth, drawHeight, flipX);
}