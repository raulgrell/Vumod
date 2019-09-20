#include <models/Model.h>
#include <models/ModelRenderer.h>
#include <platform/DisplayManager.h>

#include <scene/SceneRenderer.h>

#include <glad/glad.h>
#include <scene/SceneShader.h>

ModelRenderer::ModelRenderer(StaticShader &shader, Mat4 &projectionMatrix) : shader(shader)
{
    shader.Bind();
    shader.LoadProjectionMatrix(projectionMatrix);
    shader.Unbind();
    CHECK_GL();
}

void ModelRenderer::Render(std::unordered_map<TexturedModel *, std::vector<Entity *> *> *entities)
{
    for (auto it = entities->begin(); it != entities->end(); it++) {
        TexturedModel *model = (*it).first;

        PrepareTexturedModel(*model);

        it = entities->find(model);
        if (it != entities->end()) {
            std::vector<Entity *> *batch = it->second;

            for (auto entity : *batch) {
                PrepareInstance(*entity);
                glDrawElements(GL_TRIANGLES, model->model.GetVertexCount(), GL_UNSIGNED_INT, nullptr);
            }
        }

        UnbindTexturedModel();
    }
    CHECK_GL();
}

void ModelRenderer::PrepareTexturedModel(TexturedModel &model)
{
    IndexedModel &rawModel = model.model;
    glBindVertexArray(rawModel.GetVaoId());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    shader.LoadNumberOfRows(model.texture.GetNumberOfRows());
    if (model.texture.hasTransparency) {
        Graphics::CullBackFaces(false);
    }
    shader.LoadFakeLightingVariable(model.texture.useFakeLighting);
    shader.LoadShineVariables(model.texture.shineDamper, model.texture.reflectivity);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model.texture.GetTextureId());
    CHECK_GL();
}

void ModelRenderer::UnbindTexturedModel()
{
    Graphics::CullBackFaces(true);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
    CHECK_GL();
}

void ModelRenderer::PrepareInstance(Entity &entity)
{
    Mat4 transformationMatrix = Mat4::TRS(entity.position, entity.rotation, entity.scale);
    shader.LoadTransformationMatrix(&transformationMatrix);
    shader.LoadTextureOffset(entity.GetTextureXOffset(), entity.GetTextureYOffset());
    CHECK_GL();
}

