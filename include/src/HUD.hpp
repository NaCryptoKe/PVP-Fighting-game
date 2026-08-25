#ifndef HUD_HPP
#define HUD_HPP

#include "src/Character.hpp"
#include "src/Font.hpp"

class HUD
{
private:
    Character& player;

    float width;
    float viewportWidth;
    float viewportHeight;

public:
    HUD(Character &PLAYER, float WIDTH);

    void setViewportSize(float width, float height);
    void drawHealthBar(float x, float y, float height, bool flipped);
    void drawWinnerMessage(Font font, const char *text);
    void drawTimer(Font font, int timer, float y);
};

#endif // HUD_HPP