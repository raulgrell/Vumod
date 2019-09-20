#pragma once

#include <shaders/ShaderGL.h>
#include <light/Light.h>
#include <camera/Camera.h>

class WaterShader: public ShaderGL
{
public:
	WaterShader();

	void BindAttributes() override ;
	void GetUniformLocations() override ;

	void ConnectTextureUnits();
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
	int uTransformationMatrix = 0;
	int uViewMatrix = 0;
	int uProjectionMatrix = 0;
	int uWaterTiling = 0;
	int uReflectionTexture = 0;
	int uRefractionTexture = 0;
	int uDudvMap = 0;
	int uWaveStrength = 0;
	int uMoveFactor = 0;
	int uCameraPosition = 0;
	int uWaterReflectivity = 0;
	int uNormalMap = 0;
	int uLightColor = 0;
	int uLightPosition = 0;
	int uShineDamper = 0;
	int uReflectivity = 0;
	int uDepthMap = 0;
	int uNearPlane = 0;
	int uFarPlane = 0;
	int uSkyColor = 0;
	int uFogDensity = 0;
	int uFogGradient = 0;
};
