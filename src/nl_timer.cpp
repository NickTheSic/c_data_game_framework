#include <nl_timer.h>
#include <chrono>

std::chrono::time_point<std::chrono::system_clock> tp1, tp2;

void InitTime()
{
    tp1 = tp2 = std::chrono::system_clock::now();
}

float GetTime()
{
    tp2 = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsed = tp2-tp1;
    tp1 = tp2;

    return elapsed.count();
}
