#include <files/VFS.h>
#include <gui/VuGui.h>
#include <graphics/Loader.h>
#include <input/InputManager.h>
#include <input/MousePicker.h>
#include <loader/ObjLoader.h>
#include <loader/NormalMappingObjLoader.h>
#include <particles/ParticleMaster.h>
#include <particles/ParticleSystem.h>
#include <platform/VuWindow.h>
#include <scene/SceneShader.h>
#include <scene/Scene.h>
#include <scene/SceneRenderer.h>
#include <script/VuScript.h>
#include <util/Utils.h>
#include <water/WaterFrameBuffers.h>
#include <water/WaterShader.h>
#include <water/WaterRenderer.h>

#include <memory>

int main()
{
    VuWindow window("VuMod", 1280, 720);
    CHECK_GL();

    VuGui gui(window);
    CHECK_GL();

    VFS files;
    files.Mount("data", "data/");
    files.Unmount("data");

    VuScript scripting;

    Scene scene;
    scene.LoadFile("data/models/triple.obj");

	Mat4 projectionMatrix = Mat4::Perspective(Camera::fov, VuWindow::getAspectRatio(), Camera::near_plane, Camera::far_plane);
	// Randomness
	// srand(676452);

	Loader loader;
    Camera camera(projectionMatrix);
    SceneRenderer renderer(loader, projectionMatrix);
	ParticleMaster particleMaster(loader, renderer.GetProjectionMatrix());

    // Terrain //

    TerrainTexture backgroundTexture(loader.LoadTexture("data/res/grassy2"));
    TerrainTexture rTexture(loader.LoadTexture("data/res/mud"));
    TerrainTexture gTexture(loader.LoadTexture("data/res/pinkFlowers"));
    TerrainTexture bTexture(loader.LoadTexture("data/res/path"));
    TerrainTexturePack texturePack(backgroundTexture, rTexture, gTexture, bTexture);
    TerrainTexture blendMap(loader.LoadTexture("data/res/blendMapLake"));

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
//	// pine
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


	// Normal Mapped Models //

//	// barrel
//	RawModel* barrelRawModel = NormalMappingObjLoader::LoadOBJ("data/res/barrel", loader);
//	ModelTexture barrelModelTexture = ModelTexture(loader.LoadTexture("data/res/barrel"));
//	TexturedModel barrelModel = TexturedModel(*barrelRawModel, barrelModelTexture);
//	barrelModel.getTexture().setNormalMap(loader.LoadTexture("data/res/barrelNormal"));
//	barrelModel.getTexture().setShineDamper(10);
//	barrelModel.getTexture().setReflectivity(0.5f);

//
//	//************ENTITIES*******************
//	Entity entity(barrelModel, Vec3(Terrain::SIZE/2, 0, -Terrain::SIZE/2), 0, 0, 0, 1.0f);
//	normalMapEntities.push_back(&entity);
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
//	WaterRenderer waterRenderer(loader, waterShader, renderer.getProjectionMatrix(),
//				    renderer.getNearPlane(), renderer.getFarPlane(), buffers);
//	std::vector<WaterTile*> waters;
//
//	WaterTile water(Terrain::SIZE / 2, -Terrain::SIZE / 2, 0, Terrain::SIZE);
//	waters.push_back(&water);
//
//	Vec4 reflClipPlane(0, 1, 0, -water.getHeight() + 0.5f);
//	Vec4 refrClipPlane(0, -1, 0, water.getHeight() + 0.5f);
	Vec4 screenClipPlane(0, -1, 0, 1000000);

	// Particles //

	ParticleTexture particleAtlasTexture(loader.LoadTexture("data/res/particleAtlas"), 4, true);
	ParticleTexture particleSmokeTexture(loader.LoadTexture("data/res/smoke"), 8, false);

	ParticleSystem fireSystem(particleAtlasTexture, 50, 4, -0.01, 1.2, 5);
	fireSystem.randomizeRotation();
	fireSystem.setDirection(Vec3(0, 1, 0), 0.2f);
	fireSystem.setLifeError(0.5f);
	fireSystem.setSpeedError(0.5f);
	fireSystem.setScaleError(0.5f);

	ParticleSystem smokeSystem(particleSmokeTexture, 50, 1, -0.01f, 5, 8.0f);
	smokeSystem.setDirection(Vec3(0, 1, 0), 1.0f);
	smokeSystem.setLifeError(3.1f);
	smokeSystem.setSpeedError(1.25f);
	smokeSystem.setScaleError(2.5f);
	smokeSystem.randomizeRotation();

    CHECK_GL();

    while (window.Continue()) {
        window.Begin();

        InputManager::Update();

        // Particles //
		smokeSystem.generateParticles(Vec3(290.0f, 10.0f, -330.0f));
		fireSystem.generateParticles(Vec3(350.0f, 10.0f, -300.0f));

        particleMaster.Update(camera);
        scene.Update(window);

        renderer.Begin();
        scene.Render();

        renderer.Render(entities, normalMapEntities, terrains, lights, camera, screenClipPlane, false);
        particleMaster.Render(camera);

        gui.Begin();
        drawGui(scene);
        gui.End();

		// Reflections //

//		buffers.bindReflectionFrameBuffer();
//		float distance = 2 * (camera.getPosition().y - waters[0]->getHeight());
//		camera.getPosition().y -= distance;
//		camera.invertPitch();
//		renderer.renderScene(entities, normalMapEntities, terrains, lights, camera,
//				     reflClipPlane, true);
//		camera.getPosition().y += distance;
//		camera.invertPitch();
//
//		// Refraction //

//		buffers.bindRefractionFrameBuffer();
//		renderer.renderScene(entities, normalMapEntities, terrains, lights, camera,
//				     refrClipPlane, true);
//
//		buffers.unbindCurrentFrameBuffer();

		// Scene //

//		waterRenderer.render(waters, camera, sun);



        scripting.InterpretCommands();

        window.End();
    }
}
