#include "WaterRenderer.h"

#include <glad/glad.h>
#include <scene/SceneRenderer.h>
#include <input/InputManager.h>
#include <platform/DisplayManager.h>

static constexpr const char *DUDV_MAP = "data/res/waterDUDV";
static constexpr const char *NORMAL_MAP = "data/res/normalMap";

WaterRenderer::WaterRenderer(WaterShader &shader,
                             Mat4 &projectionMatrix,
                             float nearPlane,
                             float farPlane,
                             WaterFrameBuffers &fbos) : shader(shader), fbos(fbos)
{
    dudvTexture = Loader::LoadTexture(DUDV_MAP);
    normalMap = Loader::LoadTexture(NORMAL_MAP);
    shader.Bind();
    shader.ConnectTextureUnits();
    shader.LoadProjectionMatrix(projectionMatrix);
    shader.LoadNearPlane(nearPlane);
    shader.LoadFarPlane(farPlane);
    shader.LoadSkyColor(SceneRenderer::skyColour);
    shader.LoadFogVariables(SceneRenderer::fogDensity, SceneRenderer::fogGradient);
    shader.Unbind();

    std::vector<float> vertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f};
    auto vao = std::make_shared<VertexArray>();
    auto vbo = std::make_shared<VertexBuffer>(&vertices[0], vertices.size() * 4);
    vao->AddVertexBuffer(vbo);
    quad = std::make_unique<RawModel>(vao, 4);
}

void WaterRenderer::Render(std::vector<WaterTile> &water, Camera &camera, Light &sun)
{
    Prepare(camera, sun);
    for (WaterTile &tile : water) {
        Vec3 position {tile.GetX(), tile.GetHeight(), tile.GetZ()};
        Mat4 matrix = Mat4::TRS(position, {0.0f, 0.0f, 0.0f}, tile.GetScale());
        shader.LoadTransformationMatrix(matrix);
        glDrawArrays(GL_TRIANGLES, 0, quad->GetVertexCount());
    }
    Unbind();
}

void WaterRenderer::Prepare(Camera &camera, Light &sun)
{
    shader.Bind();
    shader.LoadViewMatrix(camera);

    shader.LoadWaterTiling(waterTiling);
    moveFactor += waveSpeed * DisplayManager::getFrameTimeSeconds();
    moveFactor = fmod(moveFactor, 1.0);
    shader.LoadMoveFactor(moveFactor);
    shader.LoadWaveStrength(waveStrength);
    shader.LoadWaterReflectivity(waterReflectivity);
    shader.LoadLight(sun);
    shader.LoadShineVariables(shineDamper, reflectivity);
    glBindVertexArray(quad->GetVaoId());
    glEnableVertexAttribArray(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbos.GetReflectionTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, fbos.GetRefractionTexture());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, dudvTexture);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, normalMap);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, fbos.GetRefractionDepthTexture());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void WaterRenderer::Unbind()
{
    glDisable(GL_BLEND);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    shader.Unbind();
}