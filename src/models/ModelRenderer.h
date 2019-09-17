#pragma once

#include <math/Mat.h>

#include <shaders/StaticShader.h>
#include <scene/Entity.h>

#include <unordered_map>

class ModelRenderer
{
public:
    ModelRenderer(StaticShader &shader, Mat4 &projectionMatrix);
    void Render(std::unordered_map<TexturedModel *, std::vector<Entity *> *> *entities);
private:
    void PrepareTexturedModel(TexturedModel &model);
    void UnbindTexturedModel();
    void PrepareInstance(Entity &entity);

    StaticShader &shader;
};

