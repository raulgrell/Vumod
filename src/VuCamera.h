#pragma once

#include "common.h"

#undef near
#undef far

struct VuCamera {
    vec3 position{0};
    vec3 rotation{0};
    float fov = 50;
    float near = 0.01;
    float far = 1000;
    mat4x4 mvp{0};

    void Update(int width, int height) {
        mat4x4 p, m;
        mat4x4_identity(m);
        mat4x4_translate(m, position[0], position[1], position[2]);

        mat4x4_rotate_X(m, m, rotation[0]);
        mat4x4_rotate_Y(m, m, rotation[1]);
        mat4x4_rotate_Z(m, m, rotation[2]);

        float ratio = width / (float) height;
        mat4x4_perspective(p, fov, ratio, near, far);
        mat4x4_mul(mvp, p, m);
    }
};

