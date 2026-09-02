#ifndef ROUND_TIMER_H
#define ROUND_TIMER_H

struct RoundTimer
{
private:
    // Number of whole seconds remaining in the game
    int second; 
    // Stores the frame rate
    float accumulator;

public:
    // Initiazlize or restart the timer with a designated time
    void reset(int time);
    // Advance the timer, by adding frame time the accumulator,
    // which eventually decrease 1 second at the end
    void update(float deltaTime);

    // Returs true if the second <= 0
    bool isExpired() const;
    // Returns the current whole second left
    int getSecondsRemaining() const;
};

#endif //ROUND_TIMER_H