#include "ParticleShader.h"

static const char *SHADER_FILE = "data/shaders/particle.glsl";

ParticleShader::ParticleShader() : ShaderGL(SHADER_FILE)
{
    BindAttributes();
    Link();
    GetUniformLocations();
}

void ParticleShader::BindAttributes()
{
    BindAttribute(0, "position");
    BindAttribute(1, "modelViewMatrix");
    BindAttribute(5, "texOffsets");
    BindAttribute(6, "blendFactor");
}

void ParticleShader::LoadNumberOfRows(float numRows)
{
    LoadFloat(uNumberOfRows, numRows);
}

void ParticleShader::LoadProjectionMatrix(const Mat4 *matrix)
{
    LoadMatrix(uProjectionMatrix, *matrix);
}

void ParticleShader::GetUniformLocations()
{
    uProjectionMatrix = GetUniformLocation("projectionMatrix");
    uNumberOfRows = GetUniformLocation("numberOfRows");
}
