#pragma once

#include <math/Mat.h>

#include <shaders/StaticShader.h>
#include <scene/Entity.h>
#include "TexturedModel.h"

#include <unordered_map>

class ModelRenderer
{
public:
    ModelRenderer(StaticShader &shader, Mat4 &projectionMatrix);
    void render(std::unordered_map<TexturedModel *, std::vector<Entity *> *> *entities);
    void prepareTexturedModel(TexturedModel &model);
    static void unbindTexturedModel();
    void prepareInstance(Entity &entity);
private:
    StaticShader &shader;
};

