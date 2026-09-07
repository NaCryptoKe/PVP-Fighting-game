#ifndef HUD_H
#define HUD_H

#include "entities/Character.h"
#include "render/Font.h"

class HUD
{
private:
    class Character& player;

    float width;
    float viewportWidth;
    float viewportHeight;

public:
    HUD(class Character &PLAYER, float WIDTH);

    void setViewportSize(float width, float height);
    void drawHealthBar(float x, float y, float height, bool flipped);
    void drawWinnerMessage(Font font, const char *text);
    void drawTimer(Font font, int timer, float y);
};

#endif // HUD_H
