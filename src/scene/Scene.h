#pragma once

#include <scene/Camera.h>
#include "platform/VuWindow.h"
#include "SceneShader.h"
#include "models/VuObject.h"
#include "texture/Material.h"

#include "common.h"

struct Scene
{
    SceneShader vs;
    Camera vc;
    std::vector<Material> materials;
    std::vector<VuObject> objects;
    Vec3 bounds_min;
    Vec3 bounds_max;
    int width = 0;
    int height = 0;
    bool wireframe = false;

    explicit Scene();

    void Update(VuWindow &vw);
    void Render();

    void LoadFile(const char *path);
    bool LoadObject(const char *path);

    VuObject Convert(const tinyobj::attrib_t &attrib,
                     const tinyobj::shape_t &shape,
                     const std::vector<tinyobj::material_t> &obj_materials,
                     const std::string &base_dir);

    void GetVertices(const tinyobj::attrib_t &attrib,
                     const tinyobj::index_t &idx0,
                     const tinyobj::index_t &idx1,
                     const tinyobj::index_t &idx2,
                     Vec3 *v);

    void GetTexCoords(const tinyobj::attrib_t &attrib,
                      const tinyobj::index_t &idx0,
                      const tinyobj::index_t &idx1,
                      const tinyobj::index_t &idx2,
                      Vec2 *tc) const;

    void GetDiffuse(const std::vector<tinyobj::material_t> &obj_materials,
                    const tinyobj::shape_t &current_shape,
                    size_t f,
                    Vec3 &diffuse) const;

    bool GetNormalsAttrib(const tinyobj::attrib_t &attrib,
                          const tinyobj::index_t &idx0,
                          const tinyobj::index_t &idx1,
                          const tinyobj::index_t &idx2,
                          Vec3 *n) const;

    bool GetNormalsSmooth(std::unordered_map<int, Vec3> &smoothVertexNormals,
                          const tinyobj::index_t &idx0,
                          const tinyobj::index_t &idx1,
                          const tinyobj::index_t &idx2,
                          Vec3 *n) const;

    bool GetNormalsComputed(Vec3 *v, Vec3 *n) const;
};