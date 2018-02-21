#ifndef TIMER_H
#define TIMER_H


class Timer {
public:
    Timer() {
        prev = 0;
    }
    bool processTick();
    Timer(double interval);
    double interval;
private:
    double prev;
};

#endif // TIMER_H
