
#include <cstdint>
#include <random>
#include <chrono>

class Random
{
public:
    static std::mt19937_64 *Engine();

    static void Seed(int seed);
    static float Float();
    static int Int(int modulo);
};

class Time
{
public:
    static uint64_t Nanoseconds();
    static uint64_t Microseconds();
    static uint64_t Milliseconds();
};