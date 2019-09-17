#pragma once

#include "scene/Camera.h"
#include <shaders/ShaderGL.h>

class SkyboxShader : public ShaderGL
{
public:
	SkyboxShader();

	void BindAttributes() override;
	void GetUniformLocations() override;

	void LoadProjectionMatrix(const Mat4 *matrix);
	void LoadViewMatrix(Camera &camera);
	void LoadFogColor(float r, float g, float b);
	void LoadBlendFactor(float blend);
	void ConnectTextureUnits();

	static constexpr float ROTATE_SPEED = 0.1f;
private:
	int location_projectionMatrix{};
	int location_viewMatrix{};
	int location_skyColor{};
	int location_cubeMap{};
	int location_cubeMap2{};
	int location_blendFactor{};
	float rotation = 0;
};
