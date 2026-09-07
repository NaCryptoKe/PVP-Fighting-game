#ifndef TIMER_H
#define TIMER_H

class Timer
{
private:
    float lastTime;
    float deltaTime;

public:
    Timer();

    void start();
    float getDeltaTime() const;

    void update();
};

#endif // TIMER_H