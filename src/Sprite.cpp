#include "src/Sprite.hpp"
#include "src/Renderer.hpp"

Sprite::Sprite()
    : texture({ 0, 0, 0}), 
    x(0), y(0),
    scale(1.0f), 
    flipX(false) {}

Sprite::Sprite(TextureData texture)
    : texture(texture), 
    x(0), y(0),  
    scale(1.0f), 
    flipX(false) {}

void Sprite::setTexture(TextureData texture) 
{ 
    texture = texture; 
}

void Sprite::setPosition(float px, float py) { x = px; y = py; }
void Sprite::setScale(float s) { scale = s; }
void Sprite::setFlip(bool flip) { flipX = flip; }

void Sprite::draw() const {
    Renderer::drawFighterSprite(
        texture, 
        x, y, 
        scale, flipX
    );
}