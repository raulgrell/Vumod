#include "WaterShader.h"

static const char* VERTEX_FILE = "data/shaders/waterVertex.glsl";
static const char* FRAGMENT_FILE = "data/shaders/waterFragment.glsl";

WaterShader::WaterShader() : ShaderGL("Water", VERTEX_FILE, FRAGMENT_FILE)
{
    BindAttributes();
    Link();
    GetUniformLocations();
}

void WaterShader::BindAttributes()
{
    BindAttribute(0, "position");
}

void WaterShader::connectTextureUnits()
{
    LoadInt(location_reflectionTexture, 0);
    LoadInt(location_refractionTexture, 1);
    LoadInt(location_dudvMap, 2);
    LoadInt(location_normalMap, 3);
    LoadInt(location_depthMap, 4);
}

void WaterShader::LoadShineVariables(float shineDamper, float reflectivity)
{
    LoadFloat(location_shineDamper, shineDamper);
    LoadFloat(location_reflectivity, reflectivity);
}

// TODO: multiple lights
void WaterShader::LoadLight(Light& sun)
{
    LoadVector(location_lightColor, sun.color);
    LoadVector(location_lightPosition, sun.position);
}

void WaterShader::LoadNearPlane(float nearPlane)
{
    LoadFloat(location_nearPlane, nearPlane);
}

void WaterShader::LoadFarPlane(float farPlane)
{
    LoadFloat(location_farPlane, farPlane);
}

// 10.0: very reflective, 0.5: quite transparent
void WaterShader::LoadWaterReflectivity(float reflectivity)
{
    LoadFloat(location_waterReflectivity, reflectivity);
}

void WaterShader::LoadWaterTiling(float factor)
{
    LoadFloat(location_waterTiling, factor);
}

void WaterShader::LoadMoveFactor(float factor)
{
    LoadFloat(location_moveFactor, factor);
}


void WaterShader::LoadWaveStrength(float factor)
{
    LoadFloat(location_waveStrength, factor);
}


void WaterShader::LoadSkyColor(Vec3 colour)
{
    LoadVector(location_skyColor, colour);
}

void WaterShader::LoadFogVariables(float density, float gradient)
{
    LoadFloat(location_fogDensity, density);
    LoadFloat(location_fogGradient, gradient);
}

void WaterShader::LoadProjectionMatrix(Mat4& matrix)
{
    LoadMatrix(location_projectionMatrix, matrix);
}

void WaterShader::LoadViewMatrix(Camera& camera)
{
	Mat4 viewMatrix = camera.GetViewMatrix();
    LoadMatrix(location_viewMatrix, viewMatrix);
    LoadVector(location_cameraPosition, camera.getPosition());
}

void WaterShader::LoadTransformationMatrix(Mat4& matrix)
{
    LoadMatrix(location_transformationMatrix, matrix);
}

void WaterShader::GetUniformLocations()
{
	location_projectionMatrix = GetUniformLocation("projectionMatrix");
	location_viewMatrix = GetUniformLocation("viewMatrix");
	location_transformationMatrix = GetUniformLocation("transformationMatrix");
	location_waterTiling = GetUniformLocation("waterTiling");
	location_reflectionTexture = GetUniformLocation("reflectionTexture");
	location_refractionTexture = GetUniformLocation("refractionTexture");
	location_dudvMap = GetUniformLocation("dudvMap");
	location_waveStrength = GetUniformLocation("waveStrength");
	location_moveFactor = GetUniformLocation("moveFactor");
	location_cameraPosition = GetUniformLocation("cameraPosition");
	location_waterReflectivity = GetUniformLocation("waterReflectivity");
	location_normalMap = GetUniformLocation("normalMap");
	location_lightColor = GetUniformLocation("lightColor");
	location_lightPosition = GetUniformLocation("lightPosition");
	location_shineDamper = GetUniformLocation("shineDamper");
	location_reflectivity = GetUniformLocation("reflectivity");
	location_depthMap = GetUniformLocation("depthMap");
	location_nearPlane = GetUniformLocation("nearPlane");
	location_farPlane = GetUniformLocation("farPlane");
	location_skyColor = GetUniformLocation("skyColor");
	location_fogDensity = GetUniformLocation("fogDensity");
	location_fogGradient = GetUniformLocation("fogGradient");
}
