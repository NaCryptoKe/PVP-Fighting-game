#include "src/Camera.hpp"

#include <cstdlib>
#include <algorithm>
#include <cmath>

// ============================================================
// Constructor
// ============================================================

Camera::Camera()
    : centerX(800.0f),
      targetCenterX(800.0f),
      zoom(1.0f),
      targetZoom(1.0f),
      viewportWidth(800.0f),
      viewportHeight(600.0f),
      stageLeft(0.0f),
      stageRight(1600.0f),
      offsetX(0.0f),
      offsetY(0.0f),
      shakeIntensity(0.0f),
      shakeDuration(0.0f),
      shakeTimer(0.0f){}


// ============================================================
// Shake
// ============================================================

void Camera::shake(float intensity, float duration)
{
    if (shakeTimer <= 0.0f || intensity > shakeIntensity)
    {
        shakeIntensity = intensity;
        shakeDuration = duration;
        shakeTimer = duration;
    }
}

// ============================================================
// Update
// ============================================================

void Camera::update(float deltaTime)
{
    if (shakeTimer > 0.0f)
    {
        shakeTimer -= deltaTime;

        float progress = shakeTimer / shakeDuration;
        float currentIntensity = shakeIntensity * progress;

        offsetX = ((rand() % 200 - 100) / 100.0f) * currentIntensity;
        offsetY = ((rand() % 200 - 100) / 100.0f) * currentIntensity;

        if (shakeTimer <= 0.0f)
        {
            shakeTimer = 0.0f;
            offsetX = 0.0f;
            offsetY = 0.0f;
        }
    }
}


// ============================================================
// Getters
// ============================================================

float Camera::getOffsetX() const
{
    return offsetX;
}

float Camera::getOffsetY() const
{
    return offsetY;
}

float Camera::getCenterX() const
{
    return centerX;
}

float Camera::getZoom() const
{
    return zoom;
}
// ============================================================
// Stage Bounds & Viewport Setup
// ============================================================

void Camera::setStageBounds(float left, float right)
{
    stageLeft = left;
    stageRight = right;
}

void Camera::setViewportSize(float width, float height) 
{
    viewportWidth = width;
    viewportHeight = height;
}

// ============================================================
// Framing Logic (midpoint, Zoom Interpolation & Clamping)
// ============================================================

void Camera::follow(float player1X, float player2X, float deltaTime)
{
    // Target center is midpoint between players
    targetCenterX = (player1X + player2X) * 0.5f;

    // Calculate required zoom based on player separation + padding (150px safety margin)
    float distance = std::abs(player1X - player2X) + 150.0f;
    float desiredVisibleWidth = std::max(distance, 600.0f);
    
    targetZoom = viewportWidth / desiredVisibleWidth;
    targetZoom = std::clamp(targetZoom, MIN_ZOOM, MAX_ZOOM);

    // Smoothly interpolate zoom and horizontal position
    zoom += (targetZoom - zoom) * 8.0f * deltaTime;
    centerX += (targetCenterX - centerX) * 8.0f * deltaTime;

    // Clamp camera center so view does not extend outside stage bounds
    float halfVisibleWidth = (viewportWidth / zoom) * 0.5f;
    float minCenterX = stageLeft + halfVisibleWidth;
    float maxCenterX = stageRight - halfVisibleWidth;

    if (minCenterX > maxCenterX)
    {
        centerX = (stageLeft + stageRight) * 0.5f;
    }
    else
    {
        centerX = std::clamp(centerX, minCenterX, maxCenterX);
    }
}