#pragma once

#include <string>

#include <math/Math.h>
#include <api/IShader.h>

class ShaderGL : IShader
{
public:
    explicit
    ShaderGL(const std::string& path);
    ShaderGL(std::string name, std::string vertexFile, std::string fragmentFile);
	~ShaderGL();

	void Link() override;
	void Bind() const override;
	void Unbind() const override;

public:
    int GetUniformLocation(const std::string &uniformName);
	void BindAttribute(int attribute, const std::string &variableName);
	void LoadInt(int location, int value);
	void LoadFloat(int location, float value);
	void LoadVector(int location, const Vec2 &vec);
	void LoadVector(int location, const Vec3 &vec);
	void LoadVector(int location, const Vec4 &vec);
	void LoadBoolean(int location, bool value);
	void LoadMatrix(int location, const Mat4 &matrix);

	int GetID() { return programID; }
    const std::string &GetName() const override { return name; }

private:
	int LoadShader(const std::string &fileName, unsigned int type);

protected:
	std::string name;
	std::string vertexFile;
	std::string fragmentFile;
	int programID;
	int vertexShaderID;
	int fragmentShaderID;
};
