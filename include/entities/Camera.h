#ifndef CAMERA_H
#define CAMERA_H

#include "entities/Character.h"

struct Camera
{
    float zoom = 1.0f;
    float minZoom = 0.5f;
    float maxZoom = 1.5f;

    // World-space point the camera is looking at (published by apply());
    // the parallax background scrolls its layers based on this.
    float centerX = 0.0f;
    float centerY = 150.0f;

    float leftLimit = -3000.0f;
    float rightLimit = 3000.0f;

    static constexpr float STAGE_LEFT = -2000.0f;
    static constexpr float STAGE_RIGHT = 2000.0f;

    void apply(Character &character1, Character &character2, float screenWidth = 1920.0f, float screenHeight = 1080.0f, float focusY = 150.0f);
    void updateBounds(Character &character1, Character &character2, float screenWidth = 1920.0f);
};

#endif // CAMERA_H