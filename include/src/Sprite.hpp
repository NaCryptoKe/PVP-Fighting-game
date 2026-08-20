#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "GL/glut.h"

#include "src/Texture.hpp"

class Sprite
{
private:
    TextureData texture;

    float x, y; // foot anchor points
    float scale;
    bool flipX;

public:
    Sprite();
    Sprite(TextureData texture);

    void setTexture(TextureData texture);   // takes texturedata struct from the texture
    void setPosition(float x, float y);
    void setScale(float scale);
    void setFlip(bool flip);

    void draw() const;  // delegates to the renderer::drawfighter internally
};
#endif // SPRITE_HPP