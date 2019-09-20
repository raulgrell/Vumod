#include "NormalMappingRenderer.h"

#include <math/Math.h>
#include <models/Model.h>
#include <scene/SceneRenderer.h>
#include <graphics/Graphics.h>

#include <glad/glad.h>

NormalMappingRenderer::NormalMappingRenderer(Mat4 &projectionMatrix)
{
    shader.Bind();
    shader.LoadProjectionMatrix(&projectionMatrix);
    shader.ConnectTextureUnits();
    shader.Unbind();
}

void NormalMappingRenderer::Render(
        std::unordered_map<TexturedModel *, std::vector<Entity *> *> &entityMap,
        Vec4 &clipPlane,
        std::vector<Light> &lights,
        Camera &camera)
{
    shader.Bind();
    Prepare(clipPlane, lights, camera);

    for (auto entityList : entityMap) {
        TexturedModel *model = entityList.first;

        PrepareTexturedModel(*model);

        auto it = entityMap.find(model);
        if (it != entityMap.end()) {
            for (auto entity : *it->second) {
                PrepareInstance(*entity);
                glDrawElements(GL_TRIANGLES, model->rawModel.vertexCount, GL_UNSIGNED_INT, nullptr);
            }
        }

        UnbindTexturedModel();
    }

    shader.Unbind();
}

void NormalMappingRenderer::PrepareTexturedModel(TexturedModel &model)
{
    RawModel &rawModel = model.rawModel;
    glBindVertexArray(rawModel.vaoId);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    ModelTexture &texture = model.texture;
    shader.LoadNumberOfRows(texture.GetNumberOfRows());
    if (texture.hasTransparency) {
        Graphics::CullBackFaces(false);
    }
    shader.LoadFakeLightingVariable(texture.useFakeLighting);
    shader.LoadShineVariables(texture.shineDamper, texture.reflectivity);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model.texture.GetTextureId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, model.texture.GetNormalMapId());
}

void NormalMappingRenderer::UnbindTexturedModel()
{
    Graphics::CullBackFaces(true);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glBindVertexArray(0);
}

void NormalMappingRenderer::PrepareInstance(Entity &entity)
{
    Mat4 transformationMatrix = Mat4::TRS(entity.position, entity.rotation, entity.scale);
    shader.LoadTransformationMatrix(transformationMatrix);
    shader.LoadTextureOffset(entity.GetTextureXOffset(), entity.GetTextureYOffset());
}

void NormalMappingRenderer::Prepare(Vec4 &clipPlane, std::vector<Light> &lights, Camera &camera)
{
    shader.LoadClipPlane(clipPlane);
    shader.LoadSkyColor(SceneRenderer::skyColour);
    shader.LoadFogVariables(SceneRenderer::fogDensity, SceneRenderer::fogGradient);
    shader.LoadLights(lights);
    shader.LoadViewMatrix(&camera);
}

