#include "ShaderGL.h"

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <scene/SceneShader.h>
#include <files/FileUtils.h>
#include <cstring>

ShaderGL::ShaderGL(const std::string &path)
{
    std::string source = file::readText(path);
    auto shaderSources = PreProcess(source);

    // Extract name from filepath
    auto lastSlash = path.find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot = path.rfind('.');
    auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
    name = path.substr(lastSlash, count);

    Compile(shaderSources.vertexSrc, shaderSources.fragmentSource);
}

ShaderGL::ShaderGL(std::string name, const std::string &vertexSrc, const std::string &fragmentSrc)
        : name(std::move(name))
{
    Compile(vertexSrc, fragmentSrc);
    CHECK_GL();
}

ShaderGL::~ShaderGL()
{
    glUseProgram(0);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    glDeleteProgram(programId);
}

ShaderGLSource ShaderGL::PreProcess(const std::string &source)
{
    ShaderGLSource sources;
    const char *vertToken = "#type vertex";
    const char *fragToken = "#type fragment";

    // Vertex
    size_t tokenPos = source.find(vertToken, 0);
    size_t eol = source.find_first_of("\r\n", tokenPos);
    size_t startPos = source.find_first_not_of("\r\n", eol);
    tokenPos = source.find(fragToken, startPos);
    int len = tokenPos - startPos;
    sources.vertexSrc = source.substr(startPos, len);

    eol = source.find_first_of("\r\n", tokenPos);
    startPos = source.find_first_not_of("\r\n", eol);
    len = tokenPos - (source.size() - 1);
    sources.fragmentSource = source.substr(startPos, len);

    return sources;
}

void ShaderGL::Compile(const std::string &vertexSrc, const std::string &fragmentSrc)
{
    programId = glCreateProgram();

    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    const GLchar *pVertexSrc = vertexSrc.c_str();
    glShaderSource(vertexShaderId, 1, &pVertexSrc, 0);
    glCompileShader(vertexShaderId);
    Graphics::CheckShader(vertexShaderId, name.c_str());

    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar *pFragmentSource = fragmentSrc.c_str();
    glShaderSource(fragmentShaderId, 1, &pFragmentSource, 0);
    glCompileShader(fragmentShaderId);
    Graphics::CheckShader(fragmentShaderId, name.c_str());

    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
}

void ShaderGL::Link()
{
    glLinkProgram(programId);
    glValidateProgram(programId);
    Graphics::CheckProgram(programId, name.c_str());

    glDetachShader(programId, fragmentShaderId);
    glDetachShader(programId, vertexShaderId);
    CHECK_GL();
}

void ShaderGL::Bind() const
{
    glUseProgram(programId);
}

void ShaderGL::Unbind() const
{
    glUseProgram(0);
}

int ShaderGL::GetUniformLocation(const std::string &uniformName)
{
    return glGetUniformLocation(programId, uniformName.c_str());
}

void ShaderGL::BindAttribute(int attribute, const std::string &variableName)
{
    glBindAttribLocation(programId, attribute, variableName.c_str());
}

void ShaderGL::LoadInt(int location, GLint value)
{
    glUniform1i(location, value);
}

void ShaderGL::LoadFloat(int location, GLfloat value)
{
    glUniform1f(location, value);
}

void ShaderGL::LoadVector(int location, const Vec2 &vec)
{
    glUniform2f(location, vec.x, vec.y);
}

void ShaderGL::LoadVector(int location, const Vec3 &vec)
{
    glUniform3f(location, vec.x, vec.y, vec.z);
}

void ShaderGL::LoadVector(int location, const Vec4 &vec)
{
    glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void ShaderGL::LoadBoolean(int location, bool value)
{
    GLfloat toLoad = (value ? 1 : 0);
    glUniform1f(location, toLoad);
}

void ShaderGL::LoadMatrix(int location, const Mat4 &matrix)
{
    glUniformMatrix4fv(location, 1, GL_TRUE, &matrix.m[0]);
}
