#include <platform/DisplayManager.h>
#include "SkyboxShader.h"

static const char* VERTEX_FILE = "data/shaders/skyboxVertexShader.glsl";
static const char* FRAGMENT_FILE = "data/shaders/skyboxFragmentShader.glsl";

SkyboxShader::SkyboxShader() : ShaderGL("Skybox", VERTEX_FILE, FRAGMENT_FILE)
{
    BindAttributes();
    Link();
    GetUniformLocations();
}

void SkyboxShader::BindAttributes()
{
    BindAttribute(0, "position");
}

void SkyboxShader::GetUniformLocations()
{
	location_projectionMatrix = GetUniformLocation("projectionMatrix");
	location_viewMatrix = GetUniformLocation("viewMatrix");
	location_skyColor = GetUniformLocation("skyColor");
	location_cubeMap = GetUniformLocation("cubeMap");
	location_cubeMap2 = GetUniformLocation("cubeMap2");
	location_blendFactor = GetUniformLocation("blendFactor");
}

void SkyboxShader::LoadProjectionMatrix(const Mat4 *matrix)
{
    LoadMatrix(location_projectionMatrix, *matrix);
}

void SkyboxShader::LoadViewMatrix(Camera &camera)
{
	Mat4 viewMatrix = camera.GetViewMatrix();
	viewMatrix.r[0].c[3] = 0.0f;
	viewMatrix.r[1].c[3] = 0.0f;
	viewMatrix.r[2].c[3] = 0.0f;
	rotation += ROTATE_SPEED * DisplayManager::getFrameTimeSeconds();
	viewMatrix *= Mat4::Rotate(math::radians(rotation), Vec3(0, 1, 0));
    LoadMatrix(location_viewMatrix, viewMatrix);
}

void SkyboxShader::LoadFogColor(float r, float g, float b)
{
    LoadVector(location_skyColor, {r, g, b});
}

void SkyboxShader::ConnectTextureUnits()
{
    LoadInt(location_cubeMap, 0);
    LoadInt(location_cubeMap2, 1);
}

void SkyboxShader::LoadBlendFactor(float blend)
{
    LoadFloat(location_blendFactor, blend);
}
