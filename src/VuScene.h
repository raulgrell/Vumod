#pragma once

#include "VuWindow.h"
#include "VuShader.h"
#include "VuCamera.h"

#include "common.h"

struct VuScene {
    VuWindow &vw;
    VuShader &vs;
    VuCamera vc;
    std::vector<tinyobj::material_t> materials;
    std::vector<VuObject> objects;
    vec3 bounds_min;
    vec3 bounds_max;

    VuScene(VuWindow &vw, VuShader &shader);

    void UpdateCamera();

    void Draw();

    void LoadFile(const char *path);

    bool LoadObject(const char *path);

    int width = 0;
    int height = 0;
};