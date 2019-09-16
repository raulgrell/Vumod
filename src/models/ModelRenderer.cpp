#include <models/RawModel.h>
#include <models/ModelRenderer.h>
#include <platform/DisplayManager.h>

#include <scene/SceneRenderer.h>

#include <glad/glad.h>
#include <scene/SceneShader.h>

ModelRenderer::ModelRenderer(StaticShader &shader, Mat4 &projectionMatrix) : shader(shader)
{
    shader.Bind();
    CheckGL();

    shader.LoadProjectionMatrix(projectionMatrix);
    shader.Unbind();
}

void ModelRenderer::render(std::unordered_map<TexturedModel *, std::vector<Entity *> *> *entities)
{
    for (auto it = entities->begin(); it != entities->end(); it++) {
        TexturedModel *model = (*it).first;

        prepareTexturedModel(*model);

        it = entities->find(model);
        if (it != entities->end()) {
            std::vector<Entity *> *batch = it->second;

            for (auto entity : *batch) {
                prepareInstance(*entity);
                glDrawElements(GL_TRIANGLES, model->rawModel.vertexCount, GL_UNSIGNED_INT, nullptr);
            }
        }

        unbindTexturedModel();
    }
}

void ModelRenderer::prepareTexturedModel(TexturedModel &model)
{
    RawModel &rawModel = model.rawModel;
    glBindVertexArray(rawModel.vaoID);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    ModelTexture &texture = model.texture;
    shader.LoadNumberOfRows(texture.numberOfRows);
    if (texture.hasTransparency) {
        SceneRenderer::disableCulling();
    }
    shader.LoadFakeLightingVariable(texture.useFakeLighting);
    shader.LoadShineVariables(texture.shineDamper, texture.reflectivity);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model.texture.getTextureID());
}

void ModelRenderer::unbindTexturedModel()
{
    SceneRenderer::enableCulling();
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

void ModelRenderer::prepareInstance(Entity &entity)
{
    Mat4 transformationMatrix = Mat4::TRS(entity.position, entity.rotation, entity.scale);

    //Vec3 pos = entity.getPosition();
    //cout << "pos = " << pos[0] << ", " << pos[1] << ", " << pos[2] << endl;
    //Mat4 t = glm::translate(Mat4(1.0f), pos);
    //Maths::printMatrix(t, "t");
    //Maths::printMatrix(transformationMatrix, "T");

    shader.LoadTransformationMatrix(&transformationMatrix);
    shader.LoadTextureOffset(entity.getTextureXOffset(), entity.getTextureYOffset());
}

