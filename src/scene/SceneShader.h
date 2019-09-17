#pragma once

#include <graphics/Buffer.h>
#include <shaders/ShaderLibrary.h>
#include <graphics/VertexArray.h>
#include <shaders/ShaderGL.h>

#include "common.h"

struct SceneShader : ShaderGL
{
    std::unique_ptr<VertexArray> vao;
    int uniform_mvp = 0;
    int uniform_tint = 0;
    int uniform_texture = 0;

    explicit
    SceneShader(const char *file_path) : ShaderGL(file_path) {}
    SceneShader(const char *name, const char *vertex_src, const char *fragment_src);

    void BindAttributes() override;
    void GetUniformLocations() override;

    void Bind() const override;
    void Unbind() const override;
};

