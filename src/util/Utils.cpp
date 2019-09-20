#include "Utils.h"

#include <sys/time.h>
#include <cstdlib>

//TODO: use c++ random funtions instead of rand()

std::mt19937_64 *Random::Engine() {
    static std::mt19937_64 gen {std::random_device{}() };
    return &gen;
}

void Random::Seed(int seed)
{
    Engine()->seed(seed);
}

float Random::Float()
{
    static std::uniform_real_distribution<float> dFloat {0, 1};
    return dFloat(*Engine());
}

int Random::Int(int modulo)
{
    static std::uniform_int_distribution<int> dInt;
    return dInt(*Engine()) % modulo;
}

uint64_t Time::Milliseconds() {
    uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    return ms;
}

uint64_t Time::Microseconds() {
    uint64_t us = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    return us;
}

uint64_t Time::Nanoseconds() {
    uint64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    return ns;
}


