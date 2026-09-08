#include "render/Renderer.h"

namespace
{
// Local-space corners of the UNIT quad every draw helper is built from.
// It is CENTERED on the origin so scaling and rotation pivot around the
// middle of the quad — which is what keeps a mirrored (negative X scale)
// sprite perfectly in place instead of sliding sideways.
constexpr float UNIT_QUAD[4][2] = {
    { -0.5f, -0.5f },
    {  0.5f, -0.5f },
    {  0.5f,  0.5f },
    { -0.5f,  0.5f },
};

// Texture coordinates matching the corner order above
constexpr float UNIT_QUAD_UV[4][2] = {
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
};
}

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

// Remove everything and set to background color
void Renderer::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Pushes the transform onto the current MODELVIEW stack. glMultMatrixf POST-
// multiplies, so the full vertex path is:
//   Projection * View * T * R * S * v
// The T*R*S part is our cg::Transform; any camera/projection set up before
// keeps working because we multiply instead of replacing.
void Renderer::drawColoredQuad(const cg::Transform& transform, float r, float g, float b, float a)
{
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glMultMatrixf(transform.toMatrix().data());

    // Filled quad (semi-transparent interior)
    glColor4f(r, g, b, a * 0.3f);
    glBegin(GL_QUADS);
        for (int i = 0; i < 4; ++i)
            glVertex2f(UNIT_QUAD[i][0], UNIT_QUAD[i][1]);
    glEnd();

    // Outline (fully opaque)
    glColor4f(r, g, b, a);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 4; ++i)
            glVertex2f(UNIT_QUAD[i][0], UNIT_QUAD[i][1]);
    glEnd();

    glPopMatrix();
}

void Renderer::drawQuad(
    float x, float y, 
    float width, float height, 
    float r, float g, float b, float a
)
{
    // Bottom-left anchored: shift the centered unit quad so its footprint
    // matches (x, y) .. (x + width, y + height).
    cg::Transform transform;
    transform.position = cg::Vec3(x + width * 0.5f, y + height * 0.5f, 0.0f);
    transform.scale    = cg::Vec3(width, height, 1.0f);

    drawColoredQuad(transform, r, g, b, a);
}

void Renderer::drawTexturedQuad(GLuint textureID, const cg::Transform& transform)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Enabling alpha blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Reset tint to full opacity white

    glPushMatrix();
    glMultMatrixf(transform.toMatrix().data());

    glBegin(GL_QUADS);
        for (int i = 0; i < 4; ++i)
        {
            glTexCoord2f(UNIT_QUAD_UV[i][0], UNIT_QUAD_UV[i][1]);  // which part of the texture
            glVertex2f(UNIT_QUAD[i][0], UNIT_QUAD[i][1]);          // where on the screen
        }
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Renderer::drawSprite(
    GLuint textureID, 
    float x, float y,
    float width, float height,
    bool flipX
)
{
    // Anchor at the bottom-left corner. The flip is a NEGATIVE X scale,
    // which mirrors the image around the quad center without moving it —
    // same footprint as the old texture-coordinate flip.
    cg::Transform transform;
    transform.position = cg::Vec3(x + width * 0.5f, y + height * 0.5f, 0.0f);
    transform.scale    = cg::Vec3(flipX ? -width : width, height, 1.0f);

    drawTexturedQuad(textureID, transform);
}

void Renderer::drawFighterSprite(
    TextureData texture, 
    float footX, float footY,
    float scale, bool flipX
)
{
    // 1. Actual rendered size based on the scale multiplier
    const float drawWidth  = texture.width  * scale;
    const float drawHeight = texture.height * scale;

    // 2. The pivot sits at the FEET: horizontally centered, on the floor.
    //    The unit quad is centered on its own origin, so lift it half a
    //    height. A negative X scale mirrors the sprite around the feet.
    cg::Transform transform;
    transform.position = cg::Vec3(footX, footY + drawHeight * 0.5f, 0.0f);
    transform.scale    = cg::Vec3(flipX ? -drawWidth : drawWidth, drawHeight, 1.0f);

    // 3. Draw
    drawTexturedQuad(texture.id, transform);
}
