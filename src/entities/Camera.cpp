#include "entities/Camera.h"

#include "GL/glut.h"
#include <iostream>
#include <algorithm>

void Camera::apply(Character &character1, Character &character2, float screenWidth, [[maybe_unused]]float screenHeight, float focusY)
{
    // 1. Calculate horizontal separation and dynamic horizontal zoom ONLY
    float absoluteHorizontal = std::abs(character1.getPositionX() - character2.getPositionX());
    float marginBufferX = 200.0f; 
    float widthRequired = absoluteHorizontal + 2.0f * marginBufferX; // times 2 for the two characters

    // Zoom is purely driven by horizontal distance
    zoom = std::clamp(screenWidth / widthRequired, minZoom, maxZoom);

    // 2. Find visible half width in world units
    float halfWidthInWorld = (screenWidth * 0.5f) / zoom;

    // 3. Find character horizontal midpoint
    float rawXMidPoint = (character1.getPositionX() + character2.getPositionX()) / 2.0f;

    // 4. Calculate valid bounds for the camera center
    float minCamX = STAGE_LEFT + halfWidthInWorld;
    float maxCamX = STAGE_RIGHT - halfWidthInWorld;

    // Clamp camera center X to keep view inside the stage
    float xCameraTarget = 0.0f;
    if (minCamX > maxCamX) xCameraTarget = (STAGE_LEFT + STAGE_RIGHT) * 0.5f;
    else xCameraTarget = std::clamp(rawXMidPoint, minCamX, maxCamX);

    // 5. Find character vertical midpoint and apply 1/4 jump height tracking
    float rawYMidPoint = (character1.getPositionY() + character2.getPositionY()) / 2.0f;
    float verticalDisplacement = std::max(0.0f, rawYMidPoint - focusY);
    
    // Camera moves up by 1/4 of vertical displacement above ground
    float yCameraTarget = focusY + (verticalDisplacement * 0.25f);

    // 6. Update character limits constrained by the clamped viewport and stage walls
    leftLimit = std::max(STAGE_LEFT, xCameraTarget - halfWidthInWorld);
    rightLimit = std::min(STAGE_RIGHT, xCameraTarget + halfWidthInWorld);

    // 7. Apply OpenGL Transformations
    // Shift screen center (halfscreenWidth, by ground) to origin
    glTranslatef(screenWidth * 0.5f, focusY, 0.0f);
    
    // Scale world uniformly by horizontal target zoom
    glScalef(zoom, zoom, 1.0f);
    
    // Shift world focus point (xMidPoint, yCameraTarget) to origin
    glTranslatef(-xCameraTarget, -yCameraTarget, 0.0f);
}

void Camera::updateBounds(Character &character1, Character &character2, float screenWidth)
{
    float xMidPoint = (character1.getPositionX() + character2.getPositionX()) * 0.5f;
    
    float absoluteHorizontal = std::abs(character1.getPositionX() - character2.getPositionX());
    float marginBufferX = 200.0f;
    float widthRequired = absoluteHorizontal + 2.0f * marginBufferX;

    float targetZoom = std::clamp(screenWidth / widthRequired, minZoom, maxZoom);

    // Calculate HALF width in world space
    float halfWidthInWorld = (screenWidth * 0.5f) / targetZoom;

    // Correctly assign left AND right limits
    leftLimit = xMidPoint - halfWidthInWorld;
    rightLimit = xMidPoint + halfWidthInWorld;
}