#include "src/HUD.hpp"
#include "src/Renderer.hpp"

#include <stdio.h>
#include <string>

HUD::HUD(Character& PLAYER, float WIDTH)
    : player(PLAYER),
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

    int health = player.getHealth();
    int maxHealth = player.getMaxHealth();

    // printf(
    //     "HUD: Health -> %d, MaxHealth -> %d\n",
    //     health,
    //     maxHealth
    // );

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

void HUD::drawWinnerMessage(
    Font font,
    const char* text
)
{
    float textWidth = font.getTextWidth(text);
    float textHeight = font.getTextHeight();

    // Center player name horizontally.
    float textX =
        (viewportWidth - textWidth) / 2.0f;

    float textY =
        (viewportHeight - textHeight) / 2.0f;


    // --------------------------------------------------------
    // "Wins"
    // --------------------------------------------------------

    const char* wins = "Wins";

    float winsWidth =
        font.getTextWidth(wins);

    float winsX =
        (viewportWidth - winsWidth) / 2.0f;

    float winsY =
        textY - textHeight * 1.5f;


    // --------------------------------------------------------
    // Render
    // --------------------------------------------------------

    font.renderText(
        text,
        textX,
        textY
    );

    font.renderText(
        wins,
        winsX,
        winsY
    );
}

void HUD::drawTimer(Font font, int timer, float y)
{
    std::string timerText = std::to_string(timer);

    float timerWidth = font.getTextWidth(timerText.c_str());
    
    float timerX = (viewportWidth - timerWidth) / 2.0f;

    font.renderText(timerText.c_str(), timerX, viewportHeight - y);
}