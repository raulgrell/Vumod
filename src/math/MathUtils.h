#pragma once

#include "Vec.h"

namespace math {
    float Barycenter(Vec3 &p1, Vec3 &p2, Vec3 &p3, Vec2 &pos);
    double radians(double degrees);

    template<typename T>
    constexpr T clamp(T v, T lo, T hi)
    {
        return (v < lo) ? lo : (hi < v) ? hi : v;
    }
}

