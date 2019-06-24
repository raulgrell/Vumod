#include "common.h"

void VuCamera::Update(int width, int height) {
    mat4x4 p, m;
    mat4x4_identity(m);
    mat4x4_translate(m, position[0], position[1], position[2]);

    mat4x4_rotate_X(m, m, (float) rotation[0]);
    mat4x4_rotate_Y(m, m, (float) rotation[1]);
    mat4x4_rotate_Z(m, m, (float) rotation[2]);

    float ratio = width / (float) height;
    mat4x4_perspective(p, 50, ratio, 0.01, 1000);
    mat4x4_mul(mvp, p, m);
}
