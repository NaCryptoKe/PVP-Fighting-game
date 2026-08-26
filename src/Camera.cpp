#include "src/Camera.hpp"

#include <cstdlib>

// ============================================================
// Constructor
// ============================================================

Camera::Camera()
    : intensity(0.0f),
      duration(0.0f),
      offsetX(0.0f),
      offsetY(0.0f) {}


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