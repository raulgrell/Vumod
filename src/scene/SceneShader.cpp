#include "SceneShader.h"

#include "glad/glad.h"

SceneShader::SceneShader(const char * path) : ShaderGL(path)
{
    BindAttributes();
    Link();
    GetUniformLocations();
    vao = std::make_unique<VertexArray>();
}

void SceneShader::BindAttributes()
{
    BindAttribute(0, "vPos");
    BindAttribute(1, "vNorm");
    BindAttribute(2, "vCol");
    BindAttribute(3, "vTexCoord");
    CHECK_GL();
}

void SceneShader::GetUniformLocations()
{
    uniformMvp =  GetUniformLocation("MVP");
    uniformTint = GetUniformLocation("Tint");
    uniformTexture =  GetUniformLocation("Texture");
    CHECK_GL();
}

void SceneShader::Bind() const
{
    glUseProgram(programId);
    vao->Bind();
}

void SceneShader::Unbind() const
{
    vao->Unbind();
    glUseProgram(0);
}

