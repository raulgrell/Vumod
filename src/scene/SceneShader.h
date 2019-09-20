#pragma once

#include <graphics/Buffer.h>
#include <shaders/ShaderLibrary.h>
#include <graphics/VertexArray.h>
#include <shaders/ShaderGL.h>

#include "common.h"

struct SceneShader : ShaderGL
{
    std::unique_ptr<VertexArray> vao;
    int uniformMvp = 0;
    int uniformTint = 0;
    int uniformTexture = 0;

    explicit SceneShader(const char *path);

    void BindAttributes() override;
    void GetUniformLocations() override;

    void Bind() const override;
    void Unbind() const override;
};

