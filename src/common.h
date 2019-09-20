#pragma once

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef VU_DEBUG
	#define HZ_ASSERT(x, ...) { if(!(x)) { VU_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define VU_ASSERT(x, ...)
#endif

#include <string>

void fatal_error(const std::string &errorString);
