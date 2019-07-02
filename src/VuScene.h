#pragma once

#include "VuWindow.h"
#include "VuShader.h"
#include "VuCamera.h"
#include "VuObject.h"
#include "VuMaterial.h"

#include "common.h"

struct VuScene
{
    VuWindow &vw;
    VuShader vs;
    VuCamera vc;
    std::vector<VuMaterial> materials;
    std::vector<VuObject> objects;
    vec3 bounds_min;
    vec3 bounds_max;
    int width = 0;
    int height = 0;
    bool wireframe = false;

    explicit VuScene(VuWindow &vw);

    void Update();
    void Draw();

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
                     vec3 *v);

    void GetTexCoords(const tinyobj::attrib_t &attrib,
                      const tinyobj::index_t &idx0,
                      const tinyobj::index_t &idx1,
                      const tinyobj::index_t &idx2,
                      vec2 *tc) const;

    void GetDiffuse(const std::vector<tinyobj::material_t> &obj_materials,
                    const tinyobj::shape_t &current_shape,
                    size_t f,
                    float *diffuse) const;

    bool GetNormalsAttrib(const tinyobj::attrib_t &attrib,
                          const tinyobj::index_t &idx0,
                          const tinyobj::index_t &idx1,
                          const tinyobj::index_t &idx2,
                          vec3 *n) const;

    bool GetNormalsSmooth(std::unordered_map<int, vec3> &smoothVertexNormals,
                          const tinyobj::index_t &idx0,
                          const tinyobj::index_t &idx1,
                          const tinyobj::index_t &idx2,
                          vec3 *n) const;

    bool GetNormalsComputed(vec3 const *v, vec3 *n) const;
};