#include "src/Camera.hpp"

#include <cstdlib>
#include <algorithm>
#include <cmath>

// ============================================================
// Constructor
// ============================================================

Camera::Camera()
    : intensity(0.0f),
      duration(0.0f),
      offsetX(0.0f),
      offsetY(0.0f),
      stageLeft(0.0f),
      stageRight(0.0f),
      viewportHeight(1920.0f),
      viewportWidth(1080.0f),
      centerX(0.0f),
      zoom(1.0f) {}


// ============================================================
// Shake
// ============================================================

void Camera::shake(float newIntensity, float newDuration)
{
    // --------------------------------------------------------
    // Ignore weaker shakes while a stronger shake is active.
    // --------------------------------------------------------

    if (newIntensity < intensity)
        return;

    intensity = newIntensity;
    duration = newDuration;
}


// ============================================================
// Update
// ============================================================

void Camera::update(float deltaTime)
{
    // --------------------------------------------------------
    // No active shake
    // --------------------------------------------------------

    if (duration <= 0.0f)
    {
        duration = 0.0f;
        intensity = 0.0f;

        offsetX = 0.0f;
        offsetY = 0.0f;

        return;
    }

    // --------------------------------------------------------
    // Decrease remaining shake duration
    // --------------------------------------------------------

    duration -= deltaTime;

    // --------------------------------------------------------
    // Shake finished
    // --------------------------------------------------------

    if (duration <= 0.0f)
    {
        duration = 0.0f;
        intensity = 0.0f;

        offsetX = 0.0f;
        offsetY = 0.0f;

        return;
    }

    // --------------------------------------------------------
    // Generate random offset
    // --------------------------------------------------------

    float randomX =
        static_cast<float>(std::rand()) / RAND_MAX;

    float randomY =
        static_cast<float>(std::rand()) / RAND_MAX;

    // Convert [0, 1] to [-1, 1]

    randomX = randomX * 2.0f - 1.0f;
    randomY = randomY * 2.0f - 1.0f;

    offsetX = randomX * intensity;
    offsetY = randomY * intensity;
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

    centerX = (stageLeft + stageRight) * 0.5f;
}

void Camera::setViewportSize(float width, float height) 
{
    viewportWidth = width;
    viewportHeight = height;
}

// ============================================================
// Framing Logic (midpoint, Zoom Interpolation & Clamping)
// ============================================================

void Camera::follow(float p1X, float p2X, float deltaTime)
{
    // 1. Calculate player distance & target zoom level
    float distance = std::abs(p1X - p2X);

    // Map distance [Near distance, Far distance] -> [0.0, 1.0]
    float t = (distance - NEAR_DISTANCE) / (FAR_DISTANCE - NEAR_DISTANCE);
    t = std::clamp(t, 0.0f, 1.0f);

    // Interpolate between MAX_ZOOM (close - up) and MIN_ZOOM (wide view)
    float targetZoom = MAX_ZOOM - t * (MAX_ZOOM - MIN_ZOOM);

    // 2. Calculate target midpoint between both players
    float targetCenterX = (p1X + p2X) * 0.5f;

    // 3. Clamp targetCenterX so the visible viewport edge never exceeds the stage's bound
    // Visible world width expands as zoom decreases
    float halfVisibleWidth = (viewportWidth / targetZoom) * 0.5f;

    // Check if the stage is wider than the visible screen area at this zoom 
    if ((stageRight - stageLeft) > (halfVisibleWidth * 2.0f))
    {
        float minCenterX = stageLeft + halfVisibleWidth;
        float maxCenterX = stageRight - halfVisibleWidth;
        targetCenterX = std::clamp(targetCenterX, minCenterX, maxCenterX);
    }
    else
    {
        // If the stage is narrower than the visible view, center on the stage midpoint
        targetCenterX = (stageLeft + stageRight) * 0.5f;
    }

    // 4. Frame-rate independent Lerp to smoothly move current state to target
    float lerpFactor = 1.0f - std::exp(-FOLLOW_SPEED * deltaTime);

    centerX += (targetCenterX - centerX) * lerpFactor;
    zoom    += (targetZoom - zoom) * lerpFactor;
}