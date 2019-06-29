#pragma once

#include "VuShader.h"

struct VuMaterial
{
    VuShader shader;

    explicit VuMaterial(const VuShader &shader);
};


