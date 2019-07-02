#pragma once

#include "VuShader.h"

struct VuMaterial
{
    const VuShader& shader;

    explicit VuMaterial(const VuShader &shader);

    void Bind() const;
    void Unbind() const;
};
