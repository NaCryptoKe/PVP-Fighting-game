#include "graphics/Sprite.h"
#include "render/Renderer.h"

Sprite::Sprite()
    : texture({ 0, 0, 0 }), 
    transform(),
    flipX(false) {}

Sprite::Sprite(TextureData texture)
    : texture(texture), 
    transform(),
    flipX(false) {}

void Sprite::setTexture(TextureData texture) 
{ 
    this->texture = texture; 
}

TextureData Sprite::getTexture() const
{
    return texture;
}

// Re-iterating, using anchor points not co-ord points
void Sprite::setPosition(float px, float py) 
{ 
    transform.position = cg::Vec3(px, py, 0.0f); 
}

void Sprite::setPosition(const cg::Vec2& position)
{
    setPosition(position.x, position.y);
}

// Uniform scale multiplier, applied on top of the texture's pixel size
void Sprite::setScale(float scale) 
{ 
    transform.scale = cg::Vec3(scale, scale, 1.0f); 
}

void Sprite::setRotationDegrees(float degrees)
{
    transform.rotationDegrees = degrees;
}

void Sprite::setFlip(bool flip) { flipX = flip; }

const cg::Transform& Sprite::getTransform() const { return transform; }

void Sprite::draw() const 
{
    if (texture.id == 0) return;   // nothing loaded yet

    // Expand the uniform multiplier into on-screen pixel size, then mirror
    // around the foot anchor through a NEGATIVE X scale (cg::Scale).
    const float drawWidth  = texture.width  * transform.scale.x;
    const float drawHeight = texture.height * transform.scale.y;

    cg::Transform quadTransform = transform;
    quadTransform.scale = cg::Vec3(flipX ? -drawWidth : drawWidth, drawHeight, 1.0f);
    quadTransform.position.y += drawHeight * 0.5f;   // foot anchor -> quad center

    Renderer::drawTexturedQuad(texture.id, quadTransform);
}
