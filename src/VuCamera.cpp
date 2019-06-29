#include "VuCamera.h"

void VuCamera::Update(int width, int height)
{
    mat4x4 p, m;
    mat4x4_identity(m);
    mat4x4_translate(m, position[0], position[1], position[2]);

    mat4x4_rotate_X(m, m, rotation[0]);
    mat4x4_rotate_Y(m, m, rotation[1]);
    mat4x4_rotate_Z(m, m, rotation[2]);

    float ratio = width / (float) height;
    mat4x4_perspective(p, fov, ratio, nearDistance, farDistance);
    mat4x4_mul(mvp, p, m);
}

void VuCamera::GetMatrix(mat4x4 m)
{
    mat4x4_dup(m, mvp);
}

