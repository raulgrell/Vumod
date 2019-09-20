#include "NormalMappingShader.h"

static const char *SHADER_FILE = "data/shaders/normal_mapped.glsl";

NormalMappingShader::NormalMappingShader() : ShaderGL(SHADER_FILE)
{
    BindAttributes();
    Link();
    GetUniformLocations();
}

void NormalMappingShader::BindAttributes()
{
    BindAttribute(0, "position");
    BindAttribute(1, "textureCoords");
    BindAttribute(2, "normal");
    BindAttribute(3, "tangent");
}

void NormalMappingShader::GetUniformLocations()
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
    location_modelTexture = GetUniformLocation("modelTexture");
    location_normalMap = GetUniformLocation("normalMap");

    for (int i = 0; i < MAX_LIGHTS; i++) {
        std::string iStr = std::to_string(i);
        location_lightPosition[i] = GetUniformLocation("lightPosition[" + iStr + "]");
        location_lightColor[i] = GetUniformLocation("lightColor[" + iStr + "]");
        location_attenuation[i] = GetUniformLocation("attenuation[" + iStr + "]");
    }
}

void NormalMappingShader::ConnectTextureUnits()
{
    LoadInt(location_modelTexture, 0);
    LoadInt(location_normalMap, 1);
}

void NormalMappingShader::LoadTransformationMatrix(const Mat4 &matrix)
{
    LoadMatrix(location_transformationMatrix, matrix);
}

void NormalMappingShader::LoadLights(const std::vector<Light> &lights)
{
    for (int i = 0; i < MAX_LIGHTS; i++) {
        if (i < lights.size()) {
            auto light = lights[i];
            LoadVector(location_lightPosition[i], light.position);
            LoadVector(location_lightColor[i], light.color);
            LoadVector(location_attenuation[i], light.attenuation);
        } else {
            Vec3 zero(0.0f, 0.0f, 0.0f);
            Vec3 unit(1.0f, 0.0f, 0.0f);
            LoadVector(location_lightPosition[i], zero);
            LoadVector(location_lightColor[i], zero);
            LoadVector(location_attenuation[i], unit);
        }
    }
}

void NormalMappingShader::LoadProjectionMatrix(const Mat4 *matrix)
{
    LoadMatrix(location_projectionMatrix, *matrix);
}

void NormalMappingShader::LoadViewMatrix(const Camera *camera)
{
    Mat4 viewMatrix = camera->GetViewMatrix();
    LoadMatrix(location_viewMatrix, viewMatrix);
}

void NormalMappingShader::LoadFakeLightingVariable(bool useFakeLighting)
{
    LoadBoolean(location_useFakeLighting, useFakeLighting);
}

void NormalMappingShader::LoadShineVariables(float damper, float reflectivity)
{
    LoadFloat(location_shineDamper, damper);
    LoadFloat(location_reflectivity, reflectivity);
}

void NormalMappingShader::LoadSkyColor(Vec3 colour)
{
    LoadVector(location_skyColor, colour);
}

void NormalMappingShader::LoadFogVariables(float density, float gradient)
{
    LoadFloat(location_fogDensity, density);
    LoadFloat(location_fogGradient, gradient);
}

void NormalMappingShader::LoadNumberOfRows(int numberOfRows)
{
    LoadFloat(location_numberOfRows, (float) numberOfRows);
}

void NormalMappingShader::LoadTextureOffset(float x, float y)
{
    Vec2 vec(x, y);
    LoadVector(location_textureOffset, vec);
}

void NormalMappingShader::LoadClipPlane(Vec4 &vec)
{
    LoadVector(location_clipPlane, vec);
}
