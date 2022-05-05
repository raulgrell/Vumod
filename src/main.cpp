#include <files/VFS.h>
#include <gui/VuGui.h>
#include <graphics/Loader.h>
#include <input/InputManager.h>
#include <loader/ObjLoader.h>
#include <particles/ParticleMaster.h>
#include <particles/ParticleSystem.h>
#include <platform/VuWindow.h>
#include <scene/Entity.h>
#include <scene/Scene.h>
#include <scene/SceneRenderer.h>
#include <script/VuScript.h>
#include <util/Utils.h>
#include <water/WaterFrameBuffers.h>
#include <water/WaterRenderer.h>

#include <memory>
#include <graphics/FrameBuffer.h>
#include <fx/PostProcessing.h>

static constexpr int WIDTH = 1280;
static constexpr int HEIGHT = 720;

int main()
{
    VuWindow window("VuMod", WIDTH, HEIGHT);
    VuGui gui(window);

    VFS files;
    files.Mount("data", "data/");

    VuScript scripting;

    Scene scene;
    scene.LoadFile("data/models/triple.obj");

    std::vector<Terrain> terrains;
    std::vector<Entity> entities;
    std::vector<Entity> normalMapEntities;
    std::vector<Light> lights;
    std::vector<WaterTile> waters;

    Mat4 projectionMatrix = Mat4::Perspective(
            Camera::fov,
            VuWindow::GetAspectRatio(),
            Camera::nearPlane,
            Camera::farPlane);

    SceneRenderer renderer(projectionMatrix);
    ParticleMaster particleMaster(projectionMatrix);

    // Terrain //

    TerrainTexture backgroundTexture(Loader::LoadTexture("data/res/grassy2"));
    TerrainTexture rTexture(Loader::LoadTexture("data/res/mud"));
    TerrainTexture gTexture(Loader::LoadTexture("data/res/pinkFlowers"));
    TerrainTexture bTexture(Loader::LoadTexture("data/res/path"));
    TerrainTexturePack texturePack(backgroundTexture, rTexture, gTexture, bTexture);

    TerrainTexture blendMap(Loader::LoadTexture("data/res/blendMapLake"));

    auto &terrain = terrains.emplace_back(0, 0, texturePack, blendMap, "");

    // Models //

    // rocks
    auto rocksRawModel = ObjLoader::LoadObj("data/res/rocks");
    auto rocksModelTexture = ModelTexture(Loader::LoadTexture("data/res/rocks"));
    auto rocksTexturedModel = TexturedModel(rocksRawModel, rocksModelTexture);

    // grass
    auto grassRawModel = ObjLoader::LoadObj("data/res/grassModel");
    auto grassModelTexture = ModelTexture(Loader::LoadTexture("data/res/grassTexture"));
    auto grassTexturedModel = TexturedModel(grassRawModel, grassModelTexture);
    grassTexturedModel.texture.SetHasTransparency(true);
    grassTexturedModel.texture.SetUseFakeLighting(true);
    grassModelTexture.SetShineDamper(1);
    grassModelTexture.SetReflectivity(0.5);

    // fern
    auto fernRawModel = ObjLoader::LoadObj("data/res/fern");
    auto fernTextureAtlas = ModelTexture(Loader::LoadTexture("data/res/fern"), 2);
    fernTextureAtlas.SetShineDamper(4);
    fernTextureAtlas.SetReflectivity(0.1);
    TexturedModel fernTexturedModel = TexturedModel(fernRawModel, fernTextureAtlas);
    fernTexturedModel.texture.SetHasTransparency(true);
    fernTexturedModel.texture.SetUseFakeLighting(true);

    // Normal Mapped Models //

    // barrel
    auto barrelRawModel = ObjLoader::LoadObj("data/res/barrel");
    auto barrelModelTexture = ModelTexture(Loader::LoadTexture("data/res/barrel"));
    auto barrelModel = TexturedModel(barrelRawModel, barrelModelTexture);
    barrelModel.texture.SetNormalMapId(Loader::LoadTexture("data/res/barrelNormal"));
    barrelModel.texture.SetShineDamper(10);
    barrelModel.texture.SetReflectivity(0.5f);

    // Entities //
    auto &entity = normalMapEntities.emplace_back(
            barrelModel,
            Vec3 {Terrain::SIZE / 2, 0, -Terrain::SIZE / 2},
            Vec3 {0, 0, 0},
            Vec3 {1});

    auto &rocks = entities.emplace_back(
            rocksTexturedModel,
            Vec3 {Terrain::SIZE / 2, 0.5f, -Terrain::SIZE / 2},
            Vec3 {0, 0, 0},
            Vec3 {Terrain::SIZE / 2});

    for (int i = 0; i < 240; i++) {
        float x = Random::Float() * Terrain::SIZE;
        float z = Random::Float() * -Terrain::SIZE;
        float y = terrain.GetHeightOfTerrain(x, z);
        if (i % 2 == 0) {
            entities.emplace_back(
                    fernTexturedModel,
                    Random::Int(4),
                    Vec3 {x, y, z},
                    Vec3 {0, Random::Float() * 360, 0},
                    Vec3 {1, 1, 1});
        } else {
            entities.emplace_back(
                    grassTexturedModel,
                    Random::Int(4),
                    Vec3 {x, y, z},
                    Vec3 {0, Random::Float() * 360, 0},
                    Vec3 {1, 1, 1});
        }
    }

    auto &sun = lights.emplace_back(
            Vec3 {10000, 10000, -10000},
            Vec3 {1.3f, 1.3f, 1.3f});

    auto lampRawModel = ObjLoader::LoadObj("data/res/lamp");
    auto lampModelTexture = ModelTexture(Loader::LoadTexture("data/res/lamp"));
    auto lampModel = TexturedModel(lampRawModel, lampModelTexture);
    lampModel.texture.SetUseFakeLighting(true);

    auto &lampEntity = entities.emplace_back(
            lampModel,
            Vec3 {0, 5, 0},
            Vec3 {0, 0, 0},
            Vec3 {1, 1, 1});

    auto &light = lights.emplace_back(
            Vec3 {0, 14, 0},
            Vec3 {3, 3, 0},
            Vec3 {1, 0.01f, 0.002f});

    // Water //

    WaterFrameBuffers buffers;
    WaterShader waterShader;
    WaterRenderer waterRenderer(
            waterShader,
            renderer.GetProjectionMatrix(),
            scene.camera.nearPlane,
            scene.camera.farPlane,
            buffers);

    auto &water = waters.emplace_back(Terrain::SIZE / 2, -Terrain::SIZE / 2, 0, Terrain::SIZE);

    Vec4 reflClipPlane {0, 1, 0, -water.GetHeight() + 0.5f};
    Vec4 refrClipPlane {0, -1, 0, water.GetHeight() + 0.5f};
    Vec4 scrnClipPlane {0, -1, 0, 10000};

    // Particles //

    ParticleTexture particleAtlasTexture(Loader::LoadTexture("data/res/particleAtlas"), 4, true);
    ParticleTexture particleSmokeTexture(Loader::LoadTexture("data/res/smoke"), 8, false);

    ParticleSystem fireSystem(particleAtlasTexture, 50, 4, -0.01, 1.2, 5);
    fireSystem.SetDirection({0, 1, 0}, 0.2f);
    fireSystem.SetLifeError(0.5f);
    fireSystem.SetSpeedError(0.5f);
    fireSystem.SetScaleError(0.5f);
    fireSystem.RandomizeRotation();

    ParticleSystem smokeSystem(particleSmokeTexture, 50, 1, -0.01f, 5, 8.0f);
    smokeSystem.SetDirection({0, 1, 0}, 1.0f);
    smokeSystem.SetLifeError(3.1f);
    smokeSystem.SetSpeedError(1.25f);
    smokeSystem.SetScaleError(2.5f);
    smokeSystem.RandomizeRotation();

    CHECK_GL();

    FrameBuffer fb(
            VuWindow::GetWidth(),
            VuWindow::GetHeight(),
            FrameBuffer::DEPTH_RENDER_BUFFER);

    while (window.Continue()) {

        // Update
        window.Begin();
        InputManager::Update();
        particleMaster.Update(scene.camera);
        scene.Update();

        smokeSystem.GenerateParticles(Vec3(290.0f, 10.0f, -330.0f));
        fireSystem.GenerateParticles(Vec3(350.0f, 10.0f, -300.0f));

        // Draw
        renderer.Begin();
        scene.Render();

        float distance = 2 * (scene.camera.GetPosition().y - waters[0].GetHeight());
        scene.camera.position.y -= distance;
        scene.camera.InvertPitch();
        renderer.Render(entities, normalMapEntities, terrains, lights, scene.camera, reflClipPlane, true);

        scene.camera.position.y += distance;
        scene.camera.InvertPitch();
        buffers.BindRefractionFrameBuffer();
        renderer.Render(entities, normalMapEntities, terrains, lights, scene.camera, refrClipPlane, true);
        WaterFrameBuffers::UnbindCurrentFrameBuffer();

        fb.Bind();
        waterRenderer.Render(waters, scene.camera, sun);

        renderer.Render(entities, normalMapEntities, terrains, lights, scene.camera, scrnClipPlane, false);
        particleMaster.Render(scene.camera);
        fb.Unbind();

        // PostProcessing::Render(fb.GetColourTexture());

        gui.Begin();
        drawGui(scene);
        gui.End();

        scripting.InterpretCommands();
        window.End();
    }

    files.Unmount("data");
}
