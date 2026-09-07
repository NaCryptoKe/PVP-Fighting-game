#ifndef SPRITE_H
#define SPRITE_H

#include "GL/glut.h"
#include "graphics/Texture.h"

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

public:
    void setTexture(TextureData texture);   // takes texturedata struct from the texture
    void setPosition(float x, float y);
    void setScale(float scale);
    void setFlip(bool flip);
    TextureData getTexture() const;

public:
    void draw() const;  // delegates to the renderer::drawfighter internally
};
#endif // SPRITE_H
