#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "GL/glut.h"

class Sprite
{
private:
    GLuint textureID;

    float x, y; // foot anchor points
    float baseWidth, baseHeight;
    float scale;
    bool flipX;

public:
    Sprite();
    Sprite(GLuint texture, float width, float height);

    void setTexture(GLuint texture);
    void setPosition(float x, float y);
    void setScale(float scale);
    void setFlip(bool flip);

    void draw() const;  // delegates to the renderer::drawfighter internally
};
#endif // SPRITE_HPP