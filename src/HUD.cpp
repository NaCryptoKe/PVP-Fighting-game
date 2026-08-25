#include "src/HUD.hpp"
#include "src/Renderer.hpp"
#include <stdio.h>

HUD::HUD (Character PLAYER, float WIDTH)
    :   player(PLAYER),
        width(WIDTH) {}

void HUD::drawHealthBar (
    float x, float y, 
    float height, bool flipped
)
{
    // Max Heath
    Renderer::drawQuad(x, y, width, height, 0.5f, 0.5f, 0.5f);

    // Current Health
    int health = player.getHealth();
    int maxHealth = player.getMaxHealth();
    printf("HUD: Health->: %d, MaxHealth->:%d", health, maxHealth);

    float currentPercent = (health / maxHealth) * width;

    Renderer::drawQuad(x, y, currentPercent, height, 0.5f, 0.0f, 0.5f);
}
