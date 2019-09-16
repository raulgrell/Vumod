#pragma once

#include "common.h"
#include <tinyobjloader/tiny_obj_loader.h>
#include "texture/VuMaterial.h"

#include <vector>
#include <string>
#include <unordered_map>

struct VuVertex
{
    Vec3 position;
    Vec3 normal;
    Vec3 color;
    Vec2 uv;

    VuVertex(const Vec3 &position, const Vec3 &normal, const Vec3 &color, const Vec2 &uv)
            : position(position), normal(normal), color(color), uv(uv) {}
};

struct VuObject
{
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
    
    std::vector<VuVertex> buffer;
    std::vector<unsigned int> indices;

    std::shared_ptr<VertexBuffer> vbo;
    std::shared_ptr<IndexBuffer> ibo;
    const VuMaterial *mtl;
    std::string name;

    size_t NumTriangles() const { return buffer.size() / 3; }
};

void CalcNormal(Vec3 N, Vec3 &v0, Vec3 &v1, Vec3 &v2);
bool hasSmoothingGroup(const tinyobj::shape_t &shape);
void computeSmoothingNormals(const tinyobj::attrib_t &attrib, const tinyobj::shape_t &shape,
                             std::unordered_map<int, Vec3> &smoothVertexNormals);