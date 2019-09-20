#pragma once

#include <scene/Camera.h>
#include <terrain/Terrain.h>
#include <light/Light.h>
#include "platform/VuWindow.h"
#include "SceneShader.h"
#include "models/VuObject.h"
#include "texture/Material.h"

#include "common.h"
#include "Entity.h"

struct Scene
{
    Camera vc;
    SceneShader vs;
    std::vector<VuObject> objects;
    std::vector<Entity> entities;
    std::vector<Terrain> terrains;
    std::vector<Light> lights;

    Vec3 boundsMin;
    Vec3 boundsMax;

    bool wireframe = false;

    explicit Scene();

    void Prepare();
    void Update();
    void Render();

    void LoadFile(const char *path);
    bool LoadObject(const char *path);
};