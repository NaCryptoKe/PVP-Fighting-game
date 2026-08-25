#include "src/RoundTimer.hpp"

void RoundTimer::reset(int time)
{
    second = time;
    accumulator = 0.0f;
}

void RoundTimer::update(float deltaTime)
{
    if (second <= 0)
        return;

    accumulator += deltaTime;
    
    while (accumulator >= 1.0f && second > 0)
    {
        second--;
        accumulator -= 1.0f;
    }
}

bool RoundTimer::isExpired() const
{
    return second <= 0;
}

int RoundTimer::getSecondsRemaining() const
{
    return second;
}