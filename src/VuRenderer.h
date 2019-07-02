#pragma once

#include "VuMaterial.h"

class VuRenderer {

    void Draw(const VertexArray &vao, const IndexBuffer &ibo, const VuMaterial & material)  const;
};
