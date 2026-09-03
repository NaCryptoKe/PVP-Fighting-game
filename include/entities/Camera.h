#ifndef CAMERA_H
#define CAMERA_H

#include "entities/Player.h"

struct Camera
{
    float zoom = 1.0f;
    float minZoom = 0.5f;
    float maxZoom = 1.5f;

    float leftLimit = -3000.0f;
    float rightLimit = 3000.0f;

    void apply(Player &player1, Player &player2, float screenWidth = 1920.0f, float screenHeight = 1080.0f, float focusY = 150.0f);
};

#endif // CAMERA_H