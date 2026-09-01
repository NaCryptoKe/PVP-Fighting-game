#include <iostream>
#include <cassert>

#include "src/RoundTimer.hpp"

void test_initialization ()
{
    RoundTimer timer;
    timer.reset(60);

    assert(timer.getSecondsRemaining() == 60);
    assert(!timer.isExpired());
}

void test_subsecond_accumulation()
{
    RoundTimer timer;
    timer.reset(5);

    // Two 60 FPS frames (~0.0166s)
    timer.update(0.0166f);
    timer.update(0.0166f);
    assert(timer.getSecondsRemaining() == 5);

    // Adding enough time to cross 1.0s
    timer.update(0.9700f);
    assert(timer.getSecondsRemaining() == 4);
}

void test_expiration_and_underflow()
{
    RoundTimer timer;
    timer.reset(2);

    timer.update(1.0f);
    assert(timer.getSecondsRemaining() == 1);
    assert(!timer.isExpired());

    timer.update(1.0f);
    assert(timer.getSecondsRemaining() == 0);
    assert(timer.isExpired());

    // Further updates after expiration must stay at 0
    timer.update(5.0f);
    assert(timer.getSecondsRemaining() == 0);
    assert(timer.isExpired());
}

void test_lag_spike()
{
    RoundTimer timer;
    timer.reset(10);

    // A single frame taking 3.2 seconds (e.g., heavy loading or lag spike)
    timer.update(3.2f);
    assert(timer.getSecondsRemaining() == 7);
    assert(!timer.isExpired());
}

int main()
{
    std::cout << "Running RoundTimer unit tests...\n";

    test_initialization();
    test_subsecond_accumulation();
    test_expiration_and_underflow();
    test_lag_spike();

    std::cout << "All test passed successfully!\n";

    return 0;
}

// Run with: g++ -std=c++17 -Wall -Wextra -Iinclude src/RoundTimer.cpp test/round_timer_test.cpp -o round_timer_test && ./round_timer_test