#pragma once

#include <graphics/Buffer.h>
#include "common.h"

struct VuShader
{
    std::unique_ptr<VertexArray> vao;
    unsigned int program = 0;

    int uniform_mvp = 0;
    int uniform_tint = 0;

    VuShader(const char *vertex_shader_source, const char *fragment_shader_source);

    void Bind() const;
    void Unbind() const;
};

#define CheckErrors() CheckErrorsInternal(__FILE__, __LINE__)
int CheckErrorsInternal(const char *file, int line);
bool CheckShader(unsigned int handle, const char *desc);
bool CheckProgram(unsigned int handle, const char *desc);

