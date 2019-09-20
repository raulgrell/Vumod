#pragma once

#include "NormalMappingShader.h"

#include <light/Light.h>
#include <math/Mat.h>
#include <scene/Entity.h>

#include <unordered_map>

class NormalMappingRenderer
{
public:
    explicit NormalMappingRenderer(Mat4 &projectionMatrix);
    void Render(
            std::unordered_map<TexturedModel *, std::vector<Entity *> *> &entityMap,
            Vec4 &clipPlane,
            std::vector<Light> &lights,
            Camera &camera);
private:
    void UnbindTexturedModel();
    void PrepareTexturedModel(TexturedModel &model);
    void PrepareInstance(Entity &entity);
    void Prepare(Vec4 &clipPlane, std::vector<Light> &lights, Camera &camera);

    NormalMappingShader shader;
};