#pragma once

#include "common.h"

#include <vector>
#include <string>
#include <unordered_map>

struct VuVertex {
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 uv;

    VuVertex(const vec3 position, const vec3 normal, const vec3 color, const vec2 uv)
            : position{position[0], position[1], position[2]},
              normal{normal[0], normal[1], normal[2]},
              color{color[0], color[1], color[2]},
              uv{uv[0], uv[1]} {}
};

struct VuObject {
    vec3 position;
    vec3 rotation;
    vec3 scale;
    std::string name;

    unsigned int vbo_id = 0;
    int material_id = 0;
    int texture_id = 0;

    std::vector<VuVertex> buffer;

    int NumTriangles() const {
        return static_cast<int>(buffer.size() / 3);
    }
};

void CalcNormal(vec3 N, const vec3 v0, const vec3 v1, const vec3 v2);
bool hasSmoothingGroup(const tinyobj::shape_t &shape);
void computeSmoothingNormals(const tinyobj::attrib_t &attrib, const tinyobj::shape_t &shape,
        std::unordered_map<int, vec3> &smoothVertexNormals);