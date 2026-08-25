#ifndef ROUND_TIMER_HPP
#define ROUND_TIMER_HPP

struct RoundTimer
{
private:
    int second;
    float accumulator;

public:
    void reset(int time);
    void update(float deltaTime);

    bool isExpired() const;
    int getSecondsRemaining() const;
};

#endif //ROUND_TIMER_HPP