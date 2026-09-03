#include "entities/Camera.h"

#include "GL/glut.h"
#include <iostream>
#include <algorithm>

void Camera::apply(Player &player1, Player &player2, float screenWidth, float screenHeight, float focusY)
{
    // 1. Calculate horizontal separation and dynamic horizontal zoom ONLY
    float absoluteHorizontal = std::abs(player1.getPositionX() - player2.getPositionX());
    float marginBufferX = 200.0f; 
    float widthRequired = absoluteHorizontal + 2.0f * marginBufferX; // times 2 for the two players

    // Zoom is purely driven by horizontal distance
    zoom = std::clamp(screenWidth / widthRequired, minZoom, maxZoom);

    // 2. Find player horizontal midpoint
    float xMidPoint = (player1.getPositionX() + player2.getPositionX()) / 2.0f;

    // 3. Find player vertical midpoint and apply 1/2 jump height tracking
    float rawYMidPoint = (player1.getPositionY() + player2.getPositionY()) / 2.0f;
    float verticalDisplacement = std::max(0.0f, rawYMidPoint);
    
    // Camera moves up by 1/2 of vertical displacement above ground
    float yCameraTarget = focusY + (verticalDisplacement * 0.5f);

    // 4. Update dynamic screen bounds based on current camera center and zoom
    float halfWidthInWorld = (screenWidth * 0.5f) / zoom;
    leftLimit = xMidPoint - halfWidthInWorld;
    rightLimit = xMidPoint + halfWidthInWorld;

    // 5. Apply OpenGL Transformations
    // Shift screen center (960, 540) to origin
    glTranslatef(screenWidth * 0.5f, screenHeight * 0.5f, 0.0f);
    
    // Scale world uniformly by horizontal target zoom
    glScalef(zoom, zoom, 1.0f);
    
    // Shift world focus point (xMidPoint, yCameraTarget) to origin
    glTranslatef(-xMidPoint, -yCameraTarget, 0.0f);
}