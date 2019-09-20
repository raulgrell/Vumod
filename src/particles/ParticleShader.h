#pragma once

#include <math/Math.h>
#include <shaders/ShaderGL.h>

class ParticleShader: public ShaderGL
{
public:
	ParticleShader();

	void GetUniformLocations() override;
	void BindAttributes() override;

	void LoadProjectionMatrix(const Mat4 *matrix);
	void LoadNumberOfRows(float numRows);
private:
	int uProjectionMatrix{};
	int uNumberOfRows{};
};

