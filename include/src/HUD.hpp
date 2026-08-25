#ifndef HUD_HPP
#define HUD_HPP

#include "src/Character.hpp"
#include "src/Font.hpp"

class HUD
{
private:
    Character player;
    float width;

public:
    HUD(Character PLAYER, float WIDTH);

    void drawHealthBar(float x, float y, float height, bool flipped);
    void drawWinnerMessage(Font font, char *text);
    void drawTimer(Font font, int timer);
};

#endif // HUD_HPP