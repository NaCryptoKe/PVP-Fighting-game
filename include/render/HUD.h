#ifndef HUD_H
#define HUD_H

#include "entities/Character.h"
#include "render/Font.h"

class HUD
{
private:
    Character& character;

    float width;
    float viewportWidth;
    float viewportHeight;

public:
    HUD(Character &CHARACTER, float WIDTH);

    void setViewportSize(float width, float height);
    void drawHealthBar(float x, float y, float height, bool flipped);

    // Draws text centered horizontally at the given y position (font based)
    void drawCentered(Font font, const char *text, float y);

    // Draws a winner title with a subtitle just below it
    void drawWinnerMessage(Font font, const char *text, const char *subtitle);

    // Draws the round countdown timer centered near the top
    void drawTimer(Font font, int timer, float y);
};

#endif // HUD_H
