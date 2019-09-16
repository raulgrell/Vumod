
#include <cstdint>
#include <cstdlib>
#include <sys/time.h>

class Random {
public:
	static void Seed(int seed);
	static float Float();
	static int Int(int modulo);
};

class Time {
public:
    static uint64_t microseconds();
};