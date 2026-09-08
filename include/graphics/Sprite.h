#ifndef SPRITE_H
#define SPRITE_H

#include "GL/glut.h"

#include "computer_graphics/core/Transform.h"
#include "graphics/Texture.h"

// A textured quad placed in the world through a cg::Transform (position,
// rotation, scale). The transform's position is the FOOT anchor: the pivot
// sits horizontally centered, on the ground line.
class Sprite
{
private:
    TextureData texture;
    cg::Transform transform;   // position / rotation / scale of the sprite
    bool flipX;

public:
    Sprite();
    explicit Sprite(TextureData texture);

    void setTexture(TextureData texture);   // takes texturedata struct from the texture
    void setPosition(float x, float y);
    void setPosition(const cg::Vec2& position);
    void setScale(float scale);             // uniform multiplier on the texture's pixel size
    void setRotationDegrees(float degrees); // rotation about Z, around the foot anchor
    void setFlip(bool flip);                // mirror horizontally (negative X scale)
    TextureData getTexture() const;
    const cg::Transform& getTransform() const;

    void draw() const;  // delegates to the renderer with the composed transform
};
#endif // SPRITE_H
