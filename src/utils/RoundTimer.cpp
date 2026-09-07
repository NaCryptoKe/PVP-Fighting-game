#include "utils/RoundTimer.h"

void RoundTimer::reset(int time)
{
    second = time;
    accumulator = 0.0f;
}


// When pausing instead of reseting the timer to reamining seconds
// We just pass 0 as delta time
void RoundTimer::update(float deltaTime)
{
    if (isExpired())
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