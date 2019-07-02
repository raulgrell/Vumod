#pragma once

#include "common.h"

struct VuCamera
{
    vec3 position {0};
    vec3 rotation {0};
    float fov = 50.f;
    float nearDistance = 0.01f;
    float farDistance = 1000.f;
    int width = 1;
    int height = 1;
    mat4x4 mvp {0};

    void UpdateMatrix();

    void GetMatrix(mat4x4 m);
};
