#include <scene/Scene.h>
#include <files/VFS.h>
#include <gui/VuGui.h>
#include <script/VuScript.h>
#include <scene/SceneShader.h>
#include <platform/VuWindow.h>

#include <memory>
#include <graphics/Loader.h>
#include <particles/ParticleMaster.h>
#include <loader/ObjLoader.h>
#include <loader/NormalMappingObjLoader.h>
#include <input/MousePicker.h>
#include <util/Utils.h>
#include <water/WaterFrameBuffers.h>
#include <water/WaterShader.h>
#include <water/WaterRenderer.h>
#include <particles/ParticleSystem.h>
#include "scene/SceneRenderer.h"

int main()
{
    VuWindow window("VuMod", 1280, 720);
    CheckGL();

    VuGui gui(window);
    CheckGL();

    VFS files;
    files.Mount("data", "data/");
    files.Unmount("data");

    VuScript scripting;

    Scene scene;
    scene.LoadFile("data/models/triple.obj");

	// Randomness
	// srand(676452);

	Loader loader;
    CheckGL();

    Camera camera;

    Mat4 projectionMatrix = Mat4::Perspective(Camera::fov, VuWindow::getAspectRatio(), Camera::near_plane, Camera::far_plane);
    SceneRenderer masterRenderer(loader, projectionMatrix);
    CheckGL();

//
//	ParticleMaster particleMaster(loader, masterRenderer.getProjectionMatrix());
//
//    // Terrain //
//
    TerrainTexture backgroundTexture(loader.LoadTexture("data/res/grassy2"));
    TerrainTexture rTexture(loader.LoadTexture("data/res/mud"));
    TerrainTexture gTexture(loader.LoadTexture("data/res/pinkFlowers"));
    TerrainTexture bTexture(loader.LoadTexture("data/res/path"));
    TerrainTexturePack texturePack(backgroundTexture, rTexture, gTexture, bTexture);
    TerrainTexture blendMap(loader.LoadTexture("data/res/blendMapLake"));
    CheckGL();

    std::vector<Terrain> terrains;
    terrains.emplace_back(0, -1, loader, texturePack, blendMap, "");

    std::vector<Entity*> entities;
	std::vector<Entity*> normalMapEntities;


	// Models //

//	// rocks
//	RawModel *rocksRawModel = ObjLoader::LoadOBJ("data/res/rocks", loader);
//	ModelTexture rocksModelTexture = ModelTexture(loader.LoadTexture("data/res/rocks"));
//	TexturedModel rocksTexturedModel = TexturedModel(*rocksRawModel, rocksModelTexture);
//
//	// pine, was tree
//	RawModel* pineRawModel = ObjLoader::LoadOBJ("data/res/pine", loader);
//	ModelTexture pineModelTexture = ModelTexture(loader.LoadTexture("data/res/pine"));
//	TexturedModel pineTexturedModel = TexturedModel(*pineRawModel, pineModelTexture);
//	pineModelTexture.shineDamper = 4;
//	pineModelTexture.reflectivity = 0.3;
//
//	// grass
//	RawModel* grassRawModel = ObjLoader::LoadOBJ("data/res/grassModel", loader);
//	ModelTexture grassModelTexture = ModelTexture(loader.LoadTexture("data/res/grassTexture"));
//	TexturedModel grassTexturedModel = TexturedModel(*grassRawModel, grassModelTexture);
//	grassTexturedModel.getTexture().setHasTransparency(true);
//	grassTexturedModel.getTexture().setUseFakeLighting(true);
//	grassModelTexture.setShineDamper(1);
//	grassModelTexture.setReflectivity(0.5);
//
//	// fern
//	RawModel* fernRawModel = ObjLoader::LoadOBJ("data/res/fern", loader);
//	ModelTexture fernTextureAtlas = ModelTexture(loader.LoadTexture("data/res/fern"));
//	fernTextureAtlas.setShineDamper(4);
//	fernTextureAtlas.setReflectivity(0.1);
//	fernTextureAtlas.setNumberOfRows(2);
//	TexturedModel fernTexturedModel = TexturedModel(*fernRawModel, fernTextureAtlas);
//	fernTexturedModel.getTexture().setHasTransparency(true);
//	fernTexturedModel.getTexture().setUseFakeLighting(true);
//
//	// flower, using fern as raw model, seems to work
//	RawModel* flowerRawModel = ObjLoader::LoadOBJ("data/res/fern", loader);
//	ModelTexture flowerTextureAtlas = ModelTexture(loader.LoadTexture("data/res/diffuse"));
//	flowerTextureAtlas.setShineDamper(5);
//	flowerTextureAtlas.setReflectivity(0.2);
//	flowerTextureAtlas.setNumberOfRows(2);
//	TexturedModel flowerTexturedModel = TexturedModel(*flowerRawModel, flowerTextureAtlas);
//	flowerTexturedModel.getTexture().setHasTransparency(true);
//	flowerTexturedModel.getTexture().setUseFakeLighting(true);

	// Normal Mapped Models //

//	// barrel
//	RawModel* barrelRawModel = NormalMappingObjLoader::LoadOBJ("data/res/barrel", loader);
//	ModelTexture barrelModelTexture = ModelTexture(loader.LoadTexture("data/res/barrel"));
//	TexturedModel barrelModel = TexturedModel(*barrelRawModel, barrelModelTexture);
//	barrelModel.getTexture().setNormalMap(loader.LoadTexture("data/res/barrelNormal"));
//	barrelModel.getTexture().setShineDamper(10);
//	barrelModel.getTexture().setReflectivity(0.5f);
//
//	// crate
//	RawModel* crateRawModel = NormalMappingObjLoader::LoadOBJ("data/res/crate", loader);
//	ModelTexture crateModelTexture = ModelTexture(loader.LoadTexture("data/res/crate"));
//	TexturedModel crateModel = TexturedModel(*crateRawModel, crateModelTexture);
//	crateModel.getTexture().setNormalMap(loader.LoadTexture("data/res/crateNormal"));
//	crateModel.getTexture().setShineDamper(10);
//	crateModel.getTexture().setReflectivity(0.5f);
//
//	// boulder
//	RawModel* boulderRawModel = NormalMappingObjLoader::LoadOBJ("data/res/boulder", loader);
//	ModelTexture boulderModelTexture = ModelTexture(loader.LoadTexture("data/res/boulder"));
//	TexturedModel boulderModel = TexturedModel(*boulderRawModel, boulderModelTexture);
//	boulderModel.getTexture().setNormalMap(loader.LoadTexture("data/res/boulderNormal"));
//	boulderModel.getTexture().setShineDamper(10);
//	boulderModel.getTexture().setReflectivity(0.5f);
//
//	//************ENTITIES*******************
//	Entity entity(barrelModel, Vec3(Terrain::SIZE/2, 0, -Terrain::SIZE/2), 0, 0, 0, 1.0f);
//	Entity entity2(boulderModel, Vec3(Terrain::SIZE/2 + 20, 0, -Terrain::SIZE/2), 0, 0, 0, 1.0f);
//	Entity entity3(crateModel, Vec3(Terrain::SIZE/2 -20, 0, -Terrain::SIZE/2), 0, 0, 0, 0.04f);
//	normalMapEntities.push_back(&entity);
//	normalMapEntities.push_back(&entity2);
//	normalMapEntities.push_back(&entity3);
//
//	for (int i = 0; i < 2*240; i++) {
//		if (i % 4 == 0) {
//			float x = Random::Float() * Terrain::SIZE;
//			float z = Random::Float() * -Terrain::SIZE;
//			float y = terrain.getHeightOfTerrain(x, z);
//			{
//				normalMapEntities.push_back(
//					new Entity(boulderModel, Vec3(x, y, z),
//						   Random::Float() * 360, Random::Float() * 360,  Random::Float() * 360,
//						   Random::Float() * 0.9f + 0.1f));
//			}
//		}
//		if (i % 3 == 0) {
//			float x = Random::Float() * Terrain::SIZE;
//			float z = Random::Float() * -Terrain::SIZE;
//			float y = terrain.getHeightOfTerrain(x, z);
//			if (y > 1) {
//				entities.push_back(
//					new Entity(fernTexturedModel, Random::Int(4), Vec3(x, y, z),
//						   0, Random::Float() * 360, 0, 0.9f));
//			}
//		}
//		if (i % 2 == 0) {
//			float x = Random::Float() * Terrain::SIZE;
//			float z = Random::Float() * -Terrain::SIZE;
//			float y = terrain.getHeightOfTerrain(x, z);
//			if (y > 1) {
//				entities.push_back(
//					new Entity(pineTexturedModel, Vec3(x, y, z),
//						   0, Random::Float() * 360, 0, Random::Float() * 0.6f + 0.8f));
//			}
//		}
//	}
//
//	Entity rocks(rocksTexturedModel, Vec3(Terrain::SIZE/2, 0.5f, -Terrain::SIZE/2),
//		     0, 0, 0, Terrain::SIZE/2);
//	entities.push_back(&rocks);

	//*******************OTHER SETUP***************

	std::vector<Light*> lights;
	Light sun = Light(Vec3(10000, 10000, -10000), Vec3(1.3f, 1.3f, 1.3f));
	lights.push_back(&sun);

//	RawModel* lampRawModel = ObjLoader::LoadOBJ("data/res/lamp", loader);
//	ModelTexture lampModelTexture = ModelTexture(loader.LoadTexture("data/res/lamp"));
//	TexturedModel lampModel = TexturedModel(*lampRawModel, lampModelTexture);
//	lampModel.getTexture().setUseFakeLighting(true);
//
//	Entity lampEntity(lampModel, Vec3(0, 5, 0), 0, 0, 0, 1);
//	entities.push_back(&lampEntity);
//
//	Light light = Light(Vec3(0, 14, 0), Vec3(3, 3, 0), Vec3(1, 0.01f, 0.002f));
//	lights.push_back(&light);
//	entities.push_back(&lampEntity);

	// Water //

//	WaterFrameBuffers buffers;
//	WaterShader waterShader;
//	WaterRenderer waterRenderer(loader, waterShader, masterRenderer.getProjectionMatrix(),
//				    masterRenderer.getNearPlane(), masterRenderer.getFarPlane(), buffers);
//	std::vector<WaterTile*> waters;
//
//	WaterTile water(Terrain::SIZE / 2, -Terrain::SIZE / 2, 0, Terrain::SIZE);
//	waters.push_back(&water);
//
//	Vec4 reflClipPlane(0, 1, 0, -water.getHeight() + 0.5f);
//	Vec4 refrClipPlane(0, -1, 0, water.getHeight() + 0.5f);
	Vec4 screenClipPlane(0, -1, 0, 1000000);

	// Particles //

//	ParticleTexture particleStarTexture(loader.LoadTexture("data/res/particleStar"), 1, true);
//	ParticleTexture particleAtlasTexture(loader.LoadTexture("data/res/particleAtlas"), 4, true);
//	ParticleTexture particleCosmicTexture(loader.LoadTexture("data/res/cosmic"), 4, true);
//	ParticleTexture particleSmokeTexture(loader.LoadTexture("data/res/smoke"), 8, false);
//
//	ParticleSystem fireSystem(particleAtlasTexture, 50, 4, -0.01, 1.2, 5);
//	fireSystem.randomizeRotation();
//	fireSystem.setDirection(Vec3(0, 1, 0), 0.2f);
//	fireSystem.setLifeError(0.5f);
//	fireSystem.setSpeedError(0.5f);
//	fireSystem.setScaleError(0.5f);
//
//	ParticleSystem smokeSystem(particleSmokeTexture, 50, 1, -0.01f, 5, 8.0f);
//	smokeSystem.setDirection(Vec3(0, 1, 0), 1.0f);
//	smokeSystem.setLifeError(3.1f);
//	smokeSystem.setSpeedError(1.25f);
//	smokeSystem.setScaleError(2.5f);
//	smokeSystem.randomizeRotation();
//
//	ParticleSystem starSystem(particleStarTexture, 100, 20, 0.8f, 7, 2);
//	starSystem.setDirection(Vec3(0, 1, 0), 0.3f);
//	starSystem.setLifeError(0.1f);
//	starSystem.setSpeedError(0.25f);
//	starSystem.setScaleError(0.5f);
//	starSystem.randomizeRotation();
//
//	ParticleSystem cosmicSystem(particleCosmicTexture, 50, 50, 0.3f, 3, 5);
//	cosmicSystem.setDirection(Vec3(0, 1, 0), 0.8f);
//	cosmicSystem.setLifeError(0.1f);
//	cosmicSystem.setSpeedError(0.25f);
//	cosmicSystem.setScaleError(0.5f);
//	cosmicSystem.randomizeRotation();
//
//	ParticleSystem atlasSystem(particleAtlasTexture, 50, 50, 0.4f, 6, 4);
//	atlasSystem.setDirection(Vec3(0, 1, 0), 0.6f);
//	atlasSystem.setLifeError(0.2f);
//	atlasSystem.setSpeedError(0.5f);
//	atlasSystem.setScaleError(0.25f);
//	atlasSystem.randomizeRotation();
    CheckGL();

    while (window.Continue()) {
        window.Begin();
        CheckGL();

        scene.Update(window);
        scene.Draw();

        gui.Begin();
        drawGui(scene);
        gui.End();

        // Particles //
//		smokeSystem.generateParticles(Vec3(290.0f, 10.0f, -330.0f));
//		fireSystem.generateParticles(Vec3(350.0f, 10.0f, -300.0f));
//		starSystem.generateParticles(Vec3(300.0f, 20.0f, -400.0f));
//		cosmicSystem.generateParticles(Vec3(420.0f, 10.0f, -270.0f));
//		atlasSystem.generateParticles(Vec3(420.0f, 10.0f, -270.0f));

//		ParticleMaster::update(camera);

		// Reflections //

//		buffers.bindReflectionFrameBuffer();
//		float distance = 2 * (camera.getPosition().y - waters[0]->getHeight());
//		camera.getPosition().y -= distance;
//		camera.invertPitch();
//		masterRenderer.renderScene(entities, normalMapEntities, terrains, lights, camera,
//				     reflClipPlane, true);
//		camera.getPosition().y += distance;
//		camera.invertPitch();
//
//		// Refraction //

//		buffers.bindRefractionFrameBuffer();
//		masterRenderer.renderScene(entities, normalMapEntities, terrains, lights, camera,
//				     refrClipPlane, true);
//
//		buffers.unbindCurrentFrameBuffer();

		// Scene //
		masterRenderer.renderScene(entities, normalMapEntities, terrains, lights, camera,
				     screenClipPlane, false);

//		waterRenderer.render(waters, camera, sun);

        // Particles //
//		particleMaster.renderParticles(camera);

        scripting.InterpretCommands();

        window.End();
    }
}
