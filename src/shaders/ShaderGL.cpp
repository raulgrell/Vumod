#include "ShaderGL.h"

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <scene/SceneShader.h>
#include <files/FileUtils.h>

ShaderGL::ShaderGL(const std::string &path)
{

}

ShaderGL::ShaderGL(std::string name, std::string vertexFile, std::string fragmentFile)
    : name(std::move(name)), vertexFile(std::move(vertexFile)), fragmentFile(std::move(fragmentFile))
{
	vertexShaderID = LoadShader(this->vertexFile, GL_VERTEX_SHADER);
	fragmentShaderID = LoadShader(this->fragmentFile, GL_FRAGMENT_SHADER);
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
    CHECK_GL();
}

ShaderGL::~ShaderGL()
{
    glUseProgram(0);
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteProgram(vertexShaderID);
	glDeleteProgram(fragmentShaderID);
	glDeleteProgram(programID);
}

void ShaderGL::Link()
{
	glLinkProgram(programID);
	glValidateProgram(programID);
	Graphics::CheckProgram(programID, "Link");
}

void ShaderGL::Bind() const
{
	glUseProgram(programID);
}

void ShaderGL::Unbind() const
{
    glUseProgram(0);
}

int ShaderGL::GetUniformLocation(const std::string &uniformName)
{
	return glGetUniformLocation(programID, uniformName.c_str());
}

void ShaderGL::BindAttribute(int attribute, const std::string &variableName)
{
	glBindAttribLocation(programID, attribute, variableName.c_str());
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

int ShaderGL::LoadShader(const std::string& fileName, GLenum type)
{
	std::string src = file::readText(fileName);
	auto pSrc = src.c_str();

	GLuint shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, &pSrc, nullptr);
	glCompileShader(shaderID);
    Graphics::CheckShader(shaderID, fileName.c_str());

	return shaderID;
}
