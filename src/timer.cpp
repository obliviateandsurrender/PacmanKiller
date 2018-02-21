#include "timer.h"
#include <GLFW/glfw3.h>

Timer::Timer(double interval) {
    prev = 0;
    this->interval = interval;
}

bool Timer::processTick() {
    double cur = glfwGetTime();
    if (cur - prev - interval >= 0) {
        prev = cur;
        return true;
    } else
        return false;
}
