#include "src/Sprite.hpp"
#include "src/Renderer.hpp"

Sprite::Sprite()
    : textureID(0), 
    x(0), y(0), 
    baseWidth(0), baseHeight(0), 
    scale(1.0f), 
    flipX(false) {}

Sprite::Sprite(GLuint texture, float baseW, float baseH)
    : textureID(texture), 
    x(0), y(0), 
    baseWidth(baseW), baseHeight(baseH), 
    scale(1.0f), 
    flipX(false) {}

void Sprite::setTexture(GLuint texture) { textureID = texture; }
void Sprite::setPosition(float px, float py) { x = px; y = py; }
void Sprite::setScale(float s) { scale = s; }
void Sprite::setFlip(bool flip) { flipX = flip; }

void Sprite::draw() const {
    Renderer::drawFighterSprite(textureID, x, y, baseWidth, baseHeight, scale, flipX);
}