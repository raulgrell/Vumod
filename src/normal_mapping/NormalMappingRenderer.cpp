#include "../models/RawModel.h"
#include "NormalMappingRenderer.h"

#include <glad/glad.h>
#include <math/Math.h>
#include <scene/SceneRenderer.h>

NormalMappingRenderer::NormalMappingRenderer(Mat4 &projectionMatrix)
{
    shader.Bind();
    shader.LoadProjectionMatrix(&projectionMatrix);
    shader.connectTextureUnits();
    shader.Unbind();
}

void NormalMappingRenderer::render(
        std::unordered_map<TexturedModel *, std::vector<Entity *> *> *entityMap,
        Vec4 &clipPlane,
        std::vector<Light *> &lights,
        Camera &camera)
{
    shader.Bind();
    prepare(clipPlane, lights, camera);

    for (auto entityList : *entityMap) {
        TexturedModel *model = entityList.first;

        prepareTexturedModel(*model);

        auto it = entityMap->find(model);
        if (it != entityMap->end()) {
            for (auto entity : *it->second) {
                prepareInstance(*entity);
                glDrawElements(GL_TRIANGLES, model->rawModel.vertexCount, GL_UNSIGNED_INT, 0);
            }
        }

        unbindTexturedModel();
    }

    shader.Unbind();
}

void NormalMappingRenderer::prepareTexturedModel(TexturedModel &model)
{
    RawModel &rawModel = model.rawModel;
    glBindVertexArray(rawModel.vaoID);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    ModelTexture &texture = model.texture;
    shader.LoadNumberOfRows(texture.numberOfRows);
    if (texture.hasTransparency) {
        SceneRenderer::disableCulling();
    }
    shader.LoadFakeLightingVariable(texture.useFakeLighting);
    shader.LoadShineVariables(texture.shineDamper, texture.reflectivity);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model.texture.getTextureID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, model.texture.getNormalMap());
}

void NormalMappingRenderer::unbindTexturedModel()
{
    SceneRenderer::enableCulling();
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glBindVertexArray(0);
}

void NormalMappingRenderer::prepareInstance(Entity &entity)
{
    Mat4 transformationMatrix = Mat4::TRS(entity.position, entity.rotation, entity.scale);
    shader.LoadTransformationMatrix(&transformationMatrix);
    shader.LoadTextureOffset(entity.getTextureXOffset(), entity.getTextureYOffset());
}

void NormalMappingRenderer::prepare(Vec4 &clipPlane, std::vector<Light *> &lights, Camera &camera)
{
    shader.LoadClipPlane(clipPlane);
    shader.LoadSkyColor(SceneRenderer::skyColour);
    shader.LoadFogVariables(SceneRenderer::fogDensity, SceneRenderer::fogGradient);
    shader.LoadLights(lights);
    shader.LoadViewMatrix(&camera);
}

