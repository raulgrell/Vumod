#include "SceneShader.h"

#include "glad/glad.h"

SceneShader::SceneShader(const char * name, const char * vertex_shader_source, const char * fragment_shader_source)
    : ShaderGL(name, vertex_shader_source, fragment_shader_source)
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
    uniform_mvp =  GetUniformLocation("MVP");
    uniform_tint = GetUniformLocation("Tint");
    uniform_texture =  GetUniformLocation("Texture");
    CHECK_GL();
}

void SceneShader::Bind() const
{
    glUseProgram(programID);
    vao->Bind();
}

void SceneShader::Unbind() const
{
    vao->Unbind();
    glUseProgram(0);
}

