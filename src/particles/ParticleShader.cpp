#include "ParticleShader.h"

static const char *VERTEX_FILE = "data/shaders/particleVShader.glsl";
static const char *FRAGMENT_FILE = "data/shaders/particleFShader.glsl";

ParticleShader::ParticleShader() : ShaderGL("Particle", VERTEX_FILE, FRAGMENT_FILE)
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
    LoadFloat(location_numberOfRows, numRows);
}

void ParticleShader::LoadProjectionMatrix(const Mat4 *matrix)
{
    LoadMatrix(location_projectionMatrix, *matrix);
}

void ParticleShader::GetUniformLocations()
{
    location_projectionMatrix = GetUniformLocation("projectionMatrix");
    location_numberOfRows = GetUniformLocation("numberOfRows");
}
