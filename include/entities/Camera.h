#ifndef CAMERA_H
#define CAMERA_H

#include "entities/Character.h"

struct Camera
{
    float zoom = 1.0f;
    float minZoom = 0.5f;
    float maxZoom = 1.5f;

    float leftLimit = -3000.0f;
    float rightLimit = 3000.0f;

    static constexpr float STAGE_LEFT = -2000.0f;
    static constexpr float STAGE_RIGHT = 2000.0f;

    void apply(Character &player1, Character &player2, float screenWidth = 1920.0f, float screenHeight = 1080.0f, float focusY = 150.0f);
    void updateBounds(Character &player1, Character &player2, float screenWidth = 1920.0f);
};

#endif // CAMERA_H