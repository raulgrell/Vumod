#include "PostProcessing.h"

void PostProcessing::Begin()
{
    glBindVertexArray(quad->GetVaoId());
    glEnableVertexAttribArray(0);
    glDisable(GL_DEPTH_TEST);
}

void PostProcessing::End()
{
    glEnable(GL_DEPTH_TEST);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

void PostProcessing::Init()
{
    auto vao = std::make_shared<VertexArray>();
    auto vbo = std::make_unique<VertexBuffer>(&POSITIONS[0], 4);
    quad = std::make_unique<RawModel>(vao, 4);
}

void PostProcessing::Render(int colourTexture)
{
    Begin();
    contrastChanger->Render(colourTexture);
    End();
}
