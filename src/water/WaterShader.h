#pragma once

#include <shaders/ShaderGL.h>
#include <light/Light.h>
#include <scene/Camera.h>

class WaterShader: public ShaderGL
{
public:
	WaterShader();

	void BindAttributes() override ;
	void GetUniformLocations() override ;

	void connectTextureUnits();
	void LoadShineVariables(float shineDamper, float reflectivity);
	void LoadLight(Light& sun);
	void LoadNearPlane(float nearPlane);
	void LoadFarPlane(float farPlane);
	void LoadWaterReflectivity(float reflectivity);
	void LoadWaterTiling(float factor);
	void LoadMoveFactor(float factor);
	void LoadWaveStrength(float factor);
	void LoadSkyColor(Vec3 colour);
	void LoadFogVariables(float density, float gradient);
	void LoadProjectionMatrix(Mat4& matrix);
	void LoadViewMatrix(Camera& camera);
	void LoadTransformationMatrix(Mat4& matrix);
private:
	int location_transformationMatrix = 0;
	int location_viewMatrix = 0;
	int location_projectionMatrix = 0;
	int location_waterTiling = 0;
	int location_reflectionTexture = 0;
	int location_refractionTexture = 0;
	int location_dudvMap = 0;
	int location_waveStrength = 0;
	int location_moveFactor = 0;
	int location_cameraPosition = 0;
	int location_waterReflectivity = 0;
	int location_normalMap = 0;
	int location_lightColor = 0;
	int location_lightPosition = 0;
	int location_shineDamper = 0;
	int location_reflectivity = 0;
	int location_depthMap = 0;
	int location_nearPlane = 0;
	int location_farPlane = 0;
	int location_skyColor = 0;
	int location_fogDensity = 0;
	int location_fogGradient = 0;
};
