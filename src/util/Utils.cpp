#include "Utils.h"

#include <sys/time.h>
#include <cstdlib>

//TODO: use c++ random funtions instead of rand()

float Random::Float()
{
	int x = rand();
	float f = x / RAND_MAX;
	return f;
}

int Random::Int(int modulo)
{
	int x = rand() % modulo;
	return x;
}

void Random::Seed(int seed)
{
	srand((unsigned int)seed);
}

uint64_t Time::Microseconds()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return ((uint64_t) tv.tv_sec) * 1000000 + tv.tv_usec;
}
