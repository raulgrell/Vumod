#pragma once

#include "common.h"
#include <tinyobjloader/tiny_obj_loader.h>
#include "texture/Material.h"
#include "Vertex.h"

#include <vector>
#include <string>
#include <unordered_map>

struct VuObject
{
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;

    Vec3 boundsMin;
    Vec3 boundsMax;
    
    std::vector<ModelVertex> buffer;
    std::vector<unsigned int> indices;

    std::shared_ptr<VertexBuffer> vbo;
    std::shared_ptr<IndexBuffer> ibo;
    const Material *mtl;
    std::string name;

    size_t NumTriangles() const { return buffer.size() / 3; }

    static VuObject Convert(const tinyobj::attrib_t &attrib,
                     const tinyobj::shape_t &shape,
                     const std::vector<tinyobj::material_t> &objMaterials,
                     const std::string &baseDir);
};
