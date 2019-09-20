#pragma once

#include <graphics/Loader.h>
#include <scene/Entity.h>
#include <terrain/Terrain.h>
#include <light/Light.h>
#include <scene/Camera.h>
#include <shaders/StaticShader.h>
#include <models/NormalMappingRenderer.h>
#include <skybox/SkyboxRenderer.h>
#include <terrain/TerrainRenderer.h>
#include <terrain/TerrainShader.h>
#include <models/ModelRenderer.h>

class SceneRenderer
{
public:
    inline static Vec3 skyColour = {157.0f / 256, 197.0f / 256, 213.0f / 256};
    inline static float fogDensity = 0.0010f;
    inline static float fogGradient = 1.5f;;

public:
    SceneRenderer(Mat4 &projectionMatrix);

    void Begin();

    void Render(
            std::vector<Entity> &entities,
            std::vector<Entity> &normalMapEntities,
            std::vector<Terrain> &terrains,
            std::vector<Light> &lights,
            Camera &camera,
            Vec4 &clipPlane,
            bool useClipping);

    void ProcessTerrain(Terrain &terrain);
    void ProcessEntity(Entity &entity);
    void ProcessNormalMapEntity(Entity &entity);

    Mat4 &GetProjectionMatrix() { return projectionMatrix; }

private:
    void Draw(
            std::vector<Light> &lights,
            Camera &camera,
            Vec4 &clipPlane,
            bool useClipping);

    Mat4 projectionMatrix;

    StaticShader staticShader;
    TerrainShader terrainShader;

    SkyboxRenderer skyboxRenderer;
    TerrainRenderer terrainRenderer;
    ModelRenderer modelRenderer;
    NormalMappingRenderer normalMappingRenderer;

    std::unordered_map<TexturedModel *, std::vector<Entity *> *> entitiesMap;
    std::unordered_map<TexturedModel *, std::vector<Entity *> *> normalMapEntitiesMap;
    std::vector<Terrain *> terrains;
};