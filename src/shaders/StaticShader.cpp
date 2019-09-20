#include "StaticShader.h"
#include "scene/SceneShader.h"


static const char* SHADER_FILE = "data/shaders/static.glsl";

StaticShader::StaticShader() : ShaderGL(SHADER_FILE)
{
    BindAttributes();
    Link();
    GetUniformLocations();
}

void StaticShader::BindAttributes()
{
    BindAttribute(0, "position");
    BindAttribute(1, "textureCoords");
    BindAttribute(2, "normal");
    CHECK_GL();
}

void StaticShader::GetUniformLocations()
{
	location_transformationMatrix = GetUniformLocation("transformationMatrix");
	location_projectionMatrix = GetUniformLocation("projectionMatrix");
	location_viewMatrix = GetUniformLocation("viewMatrix");
	location_shineDamper = GetUniformLocation("shineDamper");
	location_reflectivity = GetUniformLocation("reflectivity");
	location_useFakeLighting = GetUniformLocation("useFakeLighting");
	location_skyColor = GetUniformLocation("skyColor");
	location_fogDensity = GetUniformLocation("fogDensity");
	location_fogGradient = GetUniformLocation("fogGradient");
	location_numberOfRows = GetUniformLocation("numberOfRows");
	location_textureOffset = GetUniformLocation("textureOffset");
	location_clipPlane = GetUniformLocation("clipPlane");

	for (int i = 0; i < MAX_LIGHTS; i++) {
		std::string iStr = std::to_string(i);
		location_lightPosition[i] = GetUniformLocation("lightPosition[" + iStr + "]");
		location_lightColor[i] = GetUniformLocation("lightColor[" + iStr + "]");
		location_attenuation[i] = GetUniformLocation("attenuation[" + iStr + "]");
	}
    CHECK_GL();
}

void StaticShader::LoadTransformationMatrix(const Mat4 *matrix)
{
    LoadMatrix(location_transformationMatrix, *matrix);
    CHECK_GL();
}

void StaticShader::LoadLights(std::vector<Light> &lights)
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

void StaticShader::LoadProjectionMatrix(Mat4& matrix)
{
    LoadMatrix(location_projectionMatrix, matrix);
    CHECK_GL();
}

void StaticShader::LoadViewMatrix(Camera &camera)
{
	Mat4 viewMatrix = camera.GetViewMatrix();
    LoadMatrix(location_viewMatrix, viewMatrix);
    CHECK_GL();
}

void StaticShader::LoadFakeLightingVariable(bool useFakeLighting)
{
    LoadBoolean(location_useFakeLighting, useFakeLighting);
    CHECK_GL();
}

void StaticShader::LoadShineVariables(float damper, float reflectivity)
{
    LoadFloat(location_shineDamper, damper);
    LoadFloat(location_reflectivity, reflectivity);
    CHECK_GL();
}

void StaticShader::LoadSkyColor(float r, float g, float b)
{
    LoadVector(location_skyColor, {r, g, b});
    CHECK_GL();
}

void StaticShader::LoadFogVariables(float density, float gradient)
{
    LoadFloat(location_fogDensity, density);
    LoadFloat(location_fogGradient, gradient);
    CHECK_GL();
}

void StaticShader::LoadNumberOfRows(int numberOfRows)
{
    LoadFloat(location_numberOfRows, (float) numberOfRows);
    CHECK_GL();
}

void StaticShader::LoadTextureOffset(float x, float y)
{
    LoadVector(location_textureOffset, {x, y});
    CHECK_GL();
}

void StaticShader::LoadClipPlane(Vec4& vec)
{
    LoadVector(location_clipPlane, vec);
    CHECK_GL();
}
