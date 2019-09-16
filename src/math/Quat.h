#pragma once

#include <cmath>
#include "Mat.h"

struct Quat
{
    float x, y, z, w;

public:
    Quat() : x(0), y(0), z(0), w(1) {}
    Quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
    {
        Normalize();
    }

    void Normalize()
    {
        float mag = sqrtf(w * w + x * x + y * y + z * z);
        w /= mag;
        x /= mag;
        y /= mag;
        z /= mag;
    }

    static Quat Nlerp(Quat a, Quat b, float blend)
    {
        Quat result(0, 0, 0, 1);
        float dot = a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
        float blendI = 1.0f - blend;
        if (dot < 0) {
            result.w = blendI * a.w + blend * -b.w;
            result.x = blendI * a.x + blend * -b.x;
            result.y = blendI * a.y + blend * -b.y;
            result.z = blendI * a.z + blend * -b.z;
        } else {
            result.w = blendI * a.w + blend * b.w;
            result.x = blendI * a.x + blend * b.x;
            result.y = blendI * a.y + blend * b.y;
            result.z = blendI * a.z + blend * b.z;
        }
        result.Normalize();
        return result;
    }

    static Quat Slerp(const Quat &v0, const Quat &v1, double t)
    {
        return {0, 0, 0, 0};
    }
};


