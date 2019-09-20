#pragma once

#include <string>

#include <math/Math.h>
#include <api/IShader.h>

struct ShaderGLSource {
    std::string vertexSrc, fragmentSource;
};

class ShaderGL : IShader
{
public:
    explicit
    ShaderGL(const std::string& path);
    ShaderGL(std::string name, const std::string &vertexSrc, const std::string &fragmentSrc);
	~ShaderGL();

	void BindAttributes() override {}
	void GetUniformLocations() override {}
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

	int GetId() { return programId; }
    const std::string &GetName() const override { return name; }

private:
    ShaderGLSource PreProcess(const std::string &source);

protected:
	std::string name;
	int programId = 0;
	int vertexShaderId = 0;
	int fragmentShaderId = 0;
    void Compile(const std::string &vertexSrc, const std::string &fragmentSrc);
};
