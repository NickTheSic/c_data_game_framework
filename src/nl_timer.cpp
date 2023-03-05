#include <nl_timer.h>
#include <chrono>

std::chrono::time_point<std::chrono::system_clock> tp1 = std::chrono::system_clock::now(), tp2 = std::chrono::system_clock::now();

double GetTime()
{
    tp2 = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = tp2-tp1;
    tp1 = tp2;

    return elapsed.count();
}
