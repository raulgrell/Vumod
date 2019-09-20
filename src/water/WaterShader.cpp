#include "WaterShader.h"

static const char* SHADER_FILE = "data/shaders/water.glsl";

WaterShader::WaterShader() : ShaderGL(SHADER_FILE)
{
    BindAttributes();
    Link();
    GetUniformLocations();
}

void WaterShader::BindAttributes()
{
    BindAttribute(0, "position");
}

void WaterShader::ConnectTextureUnits()
{
    LoadInt(uReflectionTexture, 0);
    LoadInt(uRefractionTexture, 1);
    LoadInt(uDudvMap, 2);
    LoadInt(uNormalMap, 3);
    LoadInt(uDepthMap, 4);
}

void WaterShader::LoadShineVariables(float shineDamper, float reflectivity)
{
    LoadFloat(uShineDamper, shineDamper);
    LoadFloat(uReflectivity, reflectivity);
}

// TODO: multiple lights
void WaterShader::LoadLight(Light& sun)
{
    LoadVector(uLightColor, sun.color);
    LoadVector(uLightPosition, sun.position);
}

void WaterShader::LoadNearPlane(float nearPlane)
{
    LoadFloat(uNearPlane, nearPlane);
}

void WaterShader::LoadFarPlane(float farPlane)
{
    LoadFloat(uFarPlane, farPlane);
}

void WaterShader::LoadWaterReflectivity(float reflectivity)
{
    LoadFloat(uWaterReflectivity, reflectivity);
}

void WaterShader::LoadWaterTiling(float factor)
{
    LoadFloat(uWaterTiling, factor);
}

void WaterShader::LoadMoveFactor(float factor)
{
    LoadFloat(uMoveFactor, factor);
}

void WaterShader::LoadWaveStrength(float factor)
{
    LoadFloat(uWaveStrength, factor);
}

void WaterShader::LoadSkyColor(Vec3 colour)
{
    LoadVector(uSkyColor, colour);
}

void WaterShader::LoadFogVariables(float density, float gradient)
{
    LoadFloat(uFogDensity, density);
    LoadFloat(uFogGradient, gradient);
}

void WaterShader::LoadProjectionMatrix(Mat4& matrix)
{
    LoadMatrix(uProjectionMatrix, matrix);
}

void WaterShader::LoadViewMatrix(Camera& camera)
{
	Mat4 viewMatrix = camera.GetViewMatrix();
    LoadMatrix(uViewMatrix, viewMatrix);
    LoadVector(uCameraPosition, camera.GetPosition());
}

void WaterShader::LoadTransformationMatrix(Mat4& matrix)
{
    LoadMatrix(uTransformationMatrix, matrix);
}

void WaterShader::GetUniformLocations()
{
	uProjectionMatrix = GetUniformLocation("projectionMatrix");
	uViewMatrix = GetUniformLocation("viewMatrix");
	uTransformationMatrix = GetUniformLocation("transformationMatrix");
	uWaterTiling = GetUniformLocation("waterTiling");
	uReflectionTexture = GetUniformLocation("reflectionTexture");
	uRefractionTexture = GetUniformLocation("refractionTexture");
	uDudvMap = GetUniformLocation("dudvMap");
	uWaveStrength = GetUniformLocation("waveStrength");
	uMoveFactor = GetUniformLocation("moveFactor");
	uCameraPosition = GetUniformLocation("cameraPosition");
	uWaterReflectivity = GetUniformLocation("waterReflectivity");
	uNormalMap = GetUniformLocation("normalMap");
	uLightColor = GetUniformLocation("lightColor");
	uLightPosition = GetUniformLocation("lightPosition");
	uShineDamper = GetUniformLocation("shineDamper");
	uReflectivity = GetUniformLocation("reflectivity");
	uDepthMap = GetUniformLocation("depthMap");
	uNearPlane = GetUniformLocation("nearPlane");
	uFarPlane = GetUniformLocation("farPlane");
	uSkyColor = GetUniformLocation("skyColor");
	uFogDensity = GetUniformLocation("fogDensity");
	uFogGradient = GetUniformLocation("fogGradient");
}
