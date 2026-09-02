#include "core/Timer.h"

#include "GL/glut.h"

Timer::Timer() : lastTime(0.0f), deltaTime(0.0f) {}

void Timer::start()
{
    lastTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
}

float Timer::getDeltaTime() const
{
    return deltaTime;
}

void Timer::update()
{
    float currentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
    deltaTime = currentTime - lastTime;

    // Cap delta time to maximum threshold to avoid spikes
    if (deltaTime > 0.1f) deltaTime = 0.1f; // Minimum of 10FPS is allowed
    lastTime = currentTime;
}