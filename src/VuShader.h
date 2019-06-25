#pragma once

#include "common.h"

struct VuShader {
    unsigned int program = 0;
    unsigned int vao_id = 0;

    int uniform_mvp = 0;
    int uniform_tint = 0;
    int attr_position = 0;
    int attr_color = 0;
    int attr_normal = 0;
    int attr_uv = 0;

    VuShader();

    void Bind();
    void Unbind();
};

void initShader(VuShader &vs);

#define CheckErrors() CheckErrorsInternal(__FILE__, __LINE__)
int CheckErrorsInternal(const char *file, int line);

bool CheckShader(unsigned int handle, const char *desc);

bool CheckProgram(unsigned int handle, const char *desc);

