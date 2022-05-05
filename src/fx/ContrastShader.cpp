#include <glad/glad.h>
#include "ContrastShader.h"

void ContrastChanger::Render(int texture)
{
    shader.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    renderer.RenderQuad();
    shader.Unbind();
}
