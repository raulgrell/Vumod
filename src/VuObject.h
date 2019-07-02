#pragma once

#include "common.h"
#include "VuMaterial.h"

#include <vector>
#include <string>
#include <unordered_map>

struct VuVertex
{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 uv;

    VuVertex(const vec3 position, const vec3 normal, const vec3 color, const vec2 uv)
            : position {position[0], position[1], position[2]},
              normal {normal[0], normal[1], normal[2]},
              color {color[0], color[1], color[2]},
              uv {uv[0], uv[1]} {}
};

struct VuObject
{
    vec3 position {0, 0, 0};
    vec3 rotation {0, 0, 0};
    vec3 scale {1, 1, 1};
    std::vector<VuVertex> buffer;
    std::vector<unsigned int> indices;

    std::string name;
    std::shared_ptr<VertexBuffer> vbo;
    std::shared_ptr<IndexBuffer> ibo;
    std::shared_ptr<VuMaterial> mtl;
    int material_id = 0;
    int texture_id = 0;
    std::vector<const VuMaterial *> materials;

    int NumTriangles() const { return static_cast<int>(buffer.size() / 3); }
};

void CalcNormal(vec3 N, const vec3 v0, const vec3 v1, const vec3 v2);
bool hasSmoothingGroup(const tinyobj::shape_t &shape);
void computeSmoothingNormals(const tinyobj::attrib_t &attrib, const tinyobj::shape_t &shape,
                             std::unordered_map<int, vec3> &smoothVertexNormals);