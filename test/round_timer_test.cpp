#include <stdio.h>

#include "src/RoundTimer.hpp"

int main()
{
    RoundTimer timer;

    // --------------------------------------------------------
    // Start timer at 5 seconds
    // --------------------------------------------------------

    timer.reset(5);

    printf("Timer started: %d seconds\n",
           timer.getSecondsRemaining());

    // --------------------------------------------------------
    // Simulate 1 second
    // --------------------------------------------------------

    timer.update(1.0f);

    printf("After 1 second: %d seconds\n",
           timer.getSecondsRemaining());

    // --------------------------------------------------------
    // Simulate another 2 seconds
    // --------------------------------------------------------

    timer.update(2.0f);

    printf("After 3 seconds: %d seconds\n",
           timer.getSecondsRemaining());

    // --------------------------------------------------------
    // Simulate another 2 seconds
    // --------------------------------------------------------

    timer.update(2.0f);

    printf("After 5 seconds: %d seconds\n",
           timer.getSecondsRemaining());

    // --------------------------------------------------------
    // Check expiration
    // --------------------------------------------------------

    printf(
        "Expired: %s\n",
        timer.isExpired() ? "YES" : "NO"
    );

    return 0;
}