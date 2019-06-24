#pragma once

#include "common.h"

struct VuShader {
    GLuint program = 0;
    GLuint vao_id = 0;

    GLint uniform_mvp = 0;
    GLint uniform_tint = 0;
    GLint attr_position = 0;
    GLint attr_color = 0;
    GLint attr_normal = 0;
    GLint attr_uv = 0;

    void Bind();
    void Unbind();
};

void initShader(VuShader &vs);

#define CheckErrors() CheckErrorsInternal(__FILE__, __LINE__)
int CheckErrorsInternal(const char *file, int line);

bool CheckShader(GLuint handle, const char *desc);

bool CheckProgram(GLuint handle, const char *desc);

