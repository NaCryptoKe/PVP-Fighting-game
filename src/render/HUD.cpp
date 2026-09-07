#include "render/HUD.h"
#include "render/Renderer.h"

#include <stdio.h>
#include <string>

HUD::HUD(Character& CHARACTER, float WIDTH)
    : character(CHARACTER),
      width(WIDTH) {}

void HUD::setViewportSize(float width, float height)
{
    viewportHeight = height;
    viewportWidth = width;
}

void HUD::drawHealthBar(
    float x,
    float y,
    float height,
    bool flipped
)
{
    // --------------------------------------------------------
    // Maximum health background
    // --------------------------------------------------------

    Renderer::drawQuad( x, y, width, height, 0.5f, 0.5f, 0.5f );


    // --------------------------------------------------------
    // Current health
    // --------------------------------------------------------

    int health = character.getHealth();
    int maxHealth = character.getMaxHealth();

    float healthPercent = static_cast<float>(health) /
        static_cast<float>(maxHealth);

    float currentWidth = healthPercent * width;


    // --------------------------------------------------------
    // Draw health
    // --------------------------------------------------------

    if (!flipped)
    {
        Renderer::drawQuad( x, y, currentWidth, height, 1.0f, 0.0f, 0.0f );
    }
    else
    {
        Renderer::drawQuad ( x + width - currentWidth, y, currentWidth, height, 1.0f, 0.0f, 0.0f );
    }
}

void HUD::drawCentered(Font font, const char* text, float y)
{
    float textWidth = font.getTextWidth(text);
    float textX = (viewportWidth - textWidth) / 2.0f;

    font.renderText(text, textX, y);
}

void HUD::drawWinnerMessage(
    Font font,
    const char* text,
    const char* subtitle
)
{
    float textHeight = font.getTextHeight();

    // Winner title sits in the vertical center of the screen
    float titleY = (viewportHeight - textHeight) / 2.0f;

    // Subtitle (e.g. "Round Over", "Match Over") sits just below
    float subtitleY = titleY - textHeight * 1.5f;

    drawCentered(font, text, titleY);
    drawCentered(font, subtitle, subtitleY);
}

void HUD::drawTimer(Font font, int timer, float y)
{
    std::string timerText = std::to_string(timer);

    drawCentered(font, timerText.c_str(), viewportHeight - y);
}
