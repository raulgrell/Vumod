
#include "SceneRenderer.h"

#include <graphics/VertexArray.h>
#include <platform/VuWindow.h>
#include <scene/SceneShader.h>

#include <glad/glad.h>

#include <iostream>

SceneRenderer::SceneRenderer(Loader &loader, Mat4 &projectionMatrix)
        : projectionMatrix(projectionMatrix),
          staticShader(),
          terrainShader(),
          modelRenderer(staticShader, projectionMatrix),
          terrainRenderer(terrainShader, projectionMatrix),
          skyboxRenderer(loader, projectionMatrix),
          normalMappingRenderer(projectionMatrix)
{
    Graphics::CullBackFaces(true);
}

void SceneRenderer::Render(
        std::vector<Entity *> &entities,
        std::vector<Entity *> &normalMapEntities,
        std::vector<Terrain> &terrains,
        std::vector<Light *> &lights,
        Camera &camera,
        Vec4 &clipPlane,
        bool useClipping)
{
    for (Terrain terrain : terrains) {
        ProcessTerrain(terrain);
    }
    for (Entity *entity : entities) {
        ProcessEntity(*entity);
    }
    for (Entity *entity : normalMapEntities) {
        ProcessNormalMapEntity(*entity);
    }

    render(lights, camera, clipPlane, useClipping);
}

void SceneRenderer::Begin()
{
    glClearColor(skyColour.x, skyColour.y, skyColour.z, 1);
    CHECK_GL();
}

void SceneRenderer::render(
        std::vector<Light *> &lights,
        Camera &camera,
        Vec4 &clipPlane,
        bool useClipping)
{
    Graphics::EnableClipping(useClipping);

    staticShader.Bind();
    staticShader.LoadClipPlane(clipPlane);
    staticShader.LoadSkyColor(skyColour.x, skyColour.y, skyColour.z);
    staticShader.LoadFogVariables(fogDensity, fogGradient);
    staticShader.LoadLights(lights);
    staticShader.LoadViewMatrix(camera);
    modelRenderer.Render(&entitiesMap);
    staticShader.Unbind();

    normalMappingRenderer.Render(normalMapEntitiesMap, clipPlane, lights, camera);

    terrainShader.Bind();
    terrainShader.LoadClipPlane(clipPlane);
    terrainShader.LoadSkyColor(skyColour.x, skyColour.y, skyColour.z);
    terrainShader.LoadFogVariables(fogDensity, fogGradient);
    terrainShader.LoadLights(lights);
    terrainShader.LoadViewMatrix(&camera);
    terrainRenderer.Render(&terrains);
    terrainShader.Unbind();

    Graphics::EnableClipping(false);

    skyboxRenderer.Render(camera, skyColour.x, skyColour.y, skyColour.z);

    terrains.clear();
    entitiesMap.clear();
    normalMapEntitiesMap.clear();
}

void SceneRenderer::ProcessTerrain(Terrain &terrain)
{
    terrains.push_back(&terrain);
}

void SceneRenderer::ProcessEntity(Entity &entity)
{
    auto it = entitiesMap.find(&entity.model);
    if (it != entitiesMap.end()) {
        auto batch = it->second;
        batch->push_back(&entity);
    } else {
        auto *newBatch = new std::vector<Entity *>();
        newBatch->push_back(&entity);
        entitiesMap.insert(std::make_pair(&entity.model, newBatch));
    }
}

void SceneRenderer::ProcessNormalMapEntity(Entity &entity)
{
    auto it = normalMapEntitiesMap.find(&entity.model);
    if (it != normalMapEntitiesMap.end()) {
        auto batch = it->second;
        batch->push_back(&entity);
    } else {
        auto *newBatch = new std::vector<Entity *>();
        newBatch->push_back(&entity);
        normalMapEntitiesMap.insert(std::make_pair(&entity.model, newBatch));
    }
}
