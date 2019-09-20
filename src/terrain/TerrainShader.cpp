#include "TerrainShader.h"

#include <camera/Camera.h>
#include <scene/SceneShader.h>

static const char* SHADER_FILE = "data/shaders/terrain.glsl";

TerrainShader::TerrainShader() : ShaderGL(SHADER_FILE)
{
    BindAttributes();
    Link();
    GetUniformLocations();
}

void TerrainShader::BindAttributes()
{
    BindAttribute(0, "position");
    BindAttribute(1, "textureCoords");
    BindAttribute(2, "normal");
    CHECK_GL();
}

void TerrainShader::GetUniformLocations()
{
	location_transformationMatrix = GetUniformLocation("transformationMatrix");
	location_projectionMatrix = GetUniformLocation("projectionMatrix");
	location_viewMatrix = GetUniformLocation("viewMatrix");
	location_shineDamper = GetUniformLocation("shineDamper");
	location_reflectivity = GetUniformLocation("reflectivity");
	location_skyColor = GetUniformLocation("skyColor");
	location_fogDensity = GetUniformLocation("fogDensity");
	location_fogGradient = GetUniformLocation("fogGradient");
	location_backgroundTexture = GetUniformLocation("backgroundTexture");
	location_rTexture = GetUniformLocation("rTexture");
	location_gTexture = GetUniformLocation("gTexture");
	location_bTexture = GetUniformLocation("bTexture");
	location_blendMap = GetUniformLocation("blendMap");
	location_clipPlane = GetUniformLocation("clipPlane");
	CHECK_GL();

	for (int i = 0; i < MAX_LIGHTS; i++) {
		std::string iStr = std::to_string(i);
		location_lightPosition[i] = GetUniformLocation("lightPosition[" + iStr + "]");
		location_lightColor[i] = GetUniformLocation("lightColor[" + iStr + "]");
		location_attenuation[i] = GetUniformLocation("attenuation[" + iStr + "]");
		CHECK_GL();
	}
}

void TerrainShader::LoadTransformationMatrix(Mat4& matrix)
{
    LoadMatrix(location_transformationMatrix, matrix);
}

void TerrainShader::LoadLights(std::vector<Light> &lights)
{
	for (int i = 0; i < MAX_LIGHTS; i++) {
		if (i < (int)lights.size()) {
			auto light = lights[i];
            LoadVector(location_lightPosition[i], light.position);
            LoadVector(location_lightColor[i], light.color);
            LoadVector(location_attenuation[i], light.attenuation);
		} else {
			// If less than MAX_LIGHTS lights are in the lights vector,
			// load up empty information to the shaders.
			Vec3 zero(0.0f, 0.0f, 0.0f);
			Vec3 unit(1.0f, 0.0f, 0.0f);
            LoadVector(location_lightPosition[i], zero);
            LoadVector(location_lightColor[i], zero);
            LoadVector(location_attenuation[i], unit);
		}
	}
	CHECK_GL();
}

void TerrainShader::LoadProjectionMatrix(Mat4& matrix)
{
    LoadMatrix(location_projectionMatrix, matrix);
}

void TerrainShader::LoadViewMatrix(Camera *camera)
{
	Mat4 viewMatrix = camera->GetViewMatrix();
    LoadMatrix(location_viewMatrix, viewMatrix);
}

void TerrainShader::ConnectTextureUnits()
{
    LoadInt(location_backgroundTexture, 0);
    LoadInt(location_rTexture, 1);
    LoadInt(location_gTexture, 2);
    LoadInt(location_bTexture, 3);
    LoadInt(location_blendMap, 4);
}

void TerrainShader::LoadShineVariables(float damper, float reflectivity)
{
    LoadFloat(location_shineDamper, damper);
    LoadFloat(location_reflectivity, reflectivity);
}

void TerrainShader::LoadSkyColor(float r, float g, float b)
{
    LoadVector(location_skyColor, {r, g, b});
}

void TerrainShader::LoadFogVariables(float density, float gradient)
{
    LoadFloat(location_fogDensity, density);
    LoadFloat(location_fogGradient, gradient);
}

void TerrainShader::LoadClipPlane(Vec4& vec)
{
    LoadVector(location_clipPlane, vec);
}
