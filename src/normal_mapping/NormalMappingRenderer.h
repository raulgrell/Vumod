#pragma once

#include <light/Light.h>
#include <math/Mat.h>
#include "scene/Entity.h"
#include "NormalMappingShader.h"

#include <unordered_map>

class NormalMappingRenderer
{
public:
    explicit NormalMappingRenderer(Mat4 &projectionMatrix);
    void render(
            std::unordered_map<TexturedModel *,
            std::vector<Entity *> *> *entityMap,
            Vec4 &clipPlane,
            std::vector<Light *> &lights,
            Camera &camera);
    void prepareTexturedModel(TexturedModel &model);
    void unbindTexturedModel();
    void prepareInstance(Entity &entity);
    void prepare(Vec4 &clipPlane, std::vector<Light *> &lights, Camera &camera);
private:
    NormalMappingShader shader;
};