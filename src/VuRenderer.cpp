
#include "VuRenderer.h"

void VuRenderer::Draw(const VertexArray &vao, const IndexBuffer &ibo, const VuMaterial &material) const
{
    material.Bind();
    vao.Bind();
    ibo.Bind();

    glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, (void *) 0);
}
