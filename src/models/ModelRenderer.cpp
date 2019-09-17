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
                glDrawElements(GL_TRIANGLES, model->rawModel.vertexCount, GL_UNSIGNED_INT, nullptr);
            }
        }

        UnbindTexturedModel();
    }
    CHECK_GL();
}

void ModelRenderer::PrepareTexturedModel(TexturedModel &model)
{
    RawModel &rawModel = model.rawModel;
    glBindVertexArray(rawModel.vaoId);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    ModelTexture &texture = model.texture;
    shader.LoadNumberOfRows(texture.numberOfRows);
    if (texture.hasTransparency) {
        Graphics::CullBackFaces(false);
    }
    shader.LoadFakeLightingVariable(texture.useFakeLighting);
    shader.LoadShineVariables(texture.shineDamper, texture.reflectivity);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model.texture.getTextureID());
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
    shader.LoadTextureOffset(entity.getTextureXOffset(), entity.getTextureYOffset());
    CHECK_GL();
}

