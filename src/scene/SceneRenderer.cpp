
#include "SceneRenderer.h"

#include "graphics/VertexArray.h"

#include "SceneRenderer.h"

#include <glad/glad.h>
#include <iostream>
#include <platform/VuWindow.h>
#include <scene/SceneShader.h>

SceneRenderer::SceneRenderer(Loader& loader, Mat4 &projectionMatrix)
    : staticShader(),
    terrainShader(),
    modelRenderer(staticShader, projectionMatrix),
    terrainRenderer(terrainShader, projectionMatrix),
    skyboxRenderer(loader, projectionMatrix),
    normalMappingRenderer(projectionMatrix)
{
	enableCulling();
}

void SceneRenderer::renderScene(
	std::vector<Entity*>& entities,
	std::vector<Entity*>& normalMapEntities,
	std::vector<Terrain>& terrainList,
	std::vector<Light*>&lights,
	Camera& camera,
	Vec4& clipPlane,
	bool useClipping)
{
	for (Terrain terrain : terrainList) {
		processTerrain(terrain);
	}
	for (Entity* entity : entities) {
		processEntity(*entity);
	}
	for (Entity* entity : normalMapEntities) {
		processNormalMapEntity(*entity);
	}

	render(lights, camera, clipPlane, useClipping);
}

void SceneRenderer::enableCulling()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    CheckGL();
}

void SceneRenderer::disableCulling()
{
	glDisable(GL_CULL_FACE);
    CheckGL();
}

void SceneRenderer::enableClipping()
{
	glEnable(GL_CLIP_DISTANCE0);
    CheckGL();
}

void SceneRenderer::disableClipping()
{
	glDisable(GL_CLIP_DISTANCE0);
    CheckGL();
}

void SceneRenderer::Begin()
{
	glClearColor(skyColour.x, skyColour.y, skyColour.z, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CheckGL();
}

void SceneRenderer::render(
	std::vector<Light*>& lights,
	Camera& camera,
	Vec4& clipPlane,
	bool useClipping)
{
    Begin();

	if (useClipping) {
		enableClipping();
	} else {
		disableClipping();
	}

    staticShader.Bind();
	staticShader.LoadClipPlane(clipPlane);
	staticShader.LoadSkyColor(skyColour.x, skyColour.y, skyColour.z);
	staticShader.LoadFogVariables(SceneRenderer::fogDensity, SceneRenderer::fogGradient);
	staticShader.LoadLights(lights);
	staticShader.LoadViewMatrix(&camera);
	modelRenderer.render(&entitiesMap);
    staticShader.Unbind();

	normalMappingRenderer.render(&normalMapEntitiesMap, clipPlane, lights, camera);

    terrainShader.Bind();
	terrainShader.LoadClipPlane(clipPlane);
	terrainShader.LoadSkyColor(skyColour.x, skyColour.y, skyColour.z);
	terrainShader.LoadFogVariables(SceneRenderer::fogDensity, SceneRenderer::fogGradient);
	terrainShader.LoadLights(lights);
	terrainShader.LoadViewMatrix(&camera);
    terrainRenderer.Render(&terrains);
    terrainShader.Unbind();

	if (useClipping) {
		disableClipping();
	}

    skyboxRenderer.Render(&camera, skyColour.x, skyColour.y, skyColour.z);

	terrains.clear();
	entitiesMap.clear();
	normalMapEntitiesMap.clear();
}

void SceneRenderer::processTerrain(Terrain& terrain)
{
	terrains.push_back(&terrain);
}

void SceneRenderer::processEntity(Entity& entity)
{
	auto it = entitiesMap.find(&entity.model);
  	if (it != entitiesMap.end()) {
  		auto batch = it->second;
  		batch->push_back(&entity);
  	} else {
  		auto* newBatch = new std::vector<Entity*>();
  		newBatch->push_back(&entity);
  		entitiesMap.insert(std::make_pair(&entity.model, newBatch));
  	}
}

void SceneRenderer::processNormalMapEntity(Entity& entity)
{
	auto it = normalMapEntitiesMap.find(&entity.model);
	if (it != normalMapEntitiesMap.end()) {
		std::vector<Entity*>* batch = it->second;
		batch->push_back(&entity);
	} else {
		auto* newBatch = new std::vector<Entity*>();
		newBatch->push_back(&entity);
		normalMapEntitiesMap.insert(std::make_pair(&entity.model, newBatch));
	}
}
