#pragma once

#include "common.h"

struct VuWireframe {
    unsigned int program = 0;
    unsigned int vao_id = 0;

    int uniform_mvp = 0;
    int uniform_tint = 0;
    int attr_position = 0;
    int attr_color = 0;
    int attr_normal = 0;

    VuWireframe();

    void Bind();
    void Unbind();
};