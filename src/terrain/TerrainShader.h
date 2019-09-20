#pragma once

#include <math/Mat.h>
#include <light/Light.h>
#include <shaders/ShaderGL.h>

#include <vector>

// Forward declared due to dependency cycle
class Camera;

class TerrainShader : public ShaderGL
{
public:
    TerrainShader();

    void BindAttributes() override;
    void GetUniformLocations() override;

    void ConnectTextureUnits();
    void LoadShineVariables(float damper, float reflectivity);
    void LoadTransformationMatrix(Mat4 &matrix);
    void LoadLights(std::vector<Light> &lights);
    void LoadProjectionMatrix(Mat4 &matrix);
    void LoadViewMatrix(Camera *camera);
    void LoadSkyColor(float r, float g, float b);
    void LoadFogVariables(float density, float gradient);
    void LoadClipPlane(Vec4 &vec);
private:
    static constexpr int MAX_LIGHTS = 4;
    int location_transformationMatrix {};
    int location_projectionMatrix {};
    int location_viewMatrix {};
    int location_lightPosition[MAX_LIGHTS] {};
    int location_lightColor[MAX_LIGHTS] {};
    int location_attenuation[MAX_LIGHTS] {};
    int location_shineDamper {};
    int location_reflectivity {};
    int location_skyColor {};
    int location_fogDensity {};
    int location_fogGradient {};
    int location_backgroundTexture {};
    int location_rTexture {};
    int location_gTexture {};
    int location_bTexture {};
    int location_blendMap {};
    int location_clipPlane {};
};

