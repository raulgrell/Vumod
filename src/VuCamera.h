#pragma once

#include "common.h"

struct VuCamera {
    vec3 position{0};
    vec3 rotation{0};
    float fov = 50.f;
    float nearDistance = 0.01f;
    float farDistance = 1000.f;
    mat4x4 mvp{0};

    void Update(int width, int height);
    void GetMatrix(mat4x4 m);
};
