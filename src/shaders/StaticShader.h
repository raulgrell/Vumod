#pragma once

#include "ShaderGL.h"

#include <light/Light.h>
#include <scene/Camera.h>

#include <vector>

class Camera;

class StaticShader: public ShaderGL
{
public:
	StaticShader();

	void BindAttributes() override;
	void GetUniformLocations() override;

	void LoadShineVariables(float damper, float reflectivity);
	void LoadTransformationMatrix(const Mat4 *matrix);
	void LoadLights(std::vector<Light*>& lights);
	void LoadProjectionMatrix(Mat4& matrix);
	void LoadViewMatrix(Camera &camera);
	void LoadFakeLightingVariable(bool useFakeLighting);
	void LoadSkyColor(float r, float g, float b);
	void LoadFogVariables(float density, float gradient);
	void LoadNumberOfRows(int numberOfRows);
	void LoadTextureOffset(float x, float y);
	void LoadClipPlane(Vec4& vec);
private:
	static constexpr int MAX_LIGHTS = 4;
	int location_transformationMatrix{};
	int location_projectionMatrix{};
	int location_viewMatrix{};
	int location_lightPosition[MAX_LIGHTS]{};
	int location_lightColor[MAX_LIGHTS]{};
	int location_attenuation[MAX_LIGHTS]{};
	int location_shineDamper{};
	int location_reflectivity{};
	int location_useFakeLighting{};
	int location_skyColor{};
	int location_fogDensity{};
	int location_fogGradient{};
	int location_numberOfRows{};
	int location_textureOffset{};
	int location_clipPlane{};
};
