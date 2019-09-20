#include <files/FileUtils.h>
#include <iostream>
#include <algorithm>
#include <glad/glad.h>
#include "texture/VuTexture.h"
#include "models/VuObject.h"
#include "files/VFS.h"
#include "VuObject.h"

#include <cassert>
#include <scene/Scene.h>

static bool hasSmoothingGroup(const tinyobj::shape_t &shape);
static void computeSmoothingNormals(const tinyobj::attrib_t &attrib,
                                    const tinyobj::shape_t &shape, std::unordered_map<int, Vec3> &smoothVertexNormals);
static void getVertices(const tinyobj::attrib_t &attrib,
                        int idx0, int idx1, int idx2, Vec3 &boundsMin, Vec3 &boundsMax, Vec3 *v);
static void getTexCoords(const tinyobj::attrib_t &attrib, int idx0, int idx1, int idx2, Vec2 *tc);
static void getDiffuse(const std::vector<tinyobj::material_t> &obj_materials,
                       const tinyobj::shape_t &current_shape, size_t f, Vec3 &diffuse);

static bool getNormalsAttrib(const tinyobj::attrib_t &attrib, int idx0, int idx1, int idx2, Vec3 *n);
static bool getNormalsSmooth(std::unordered_map<int, Vec3> &smoothVertexNormals,
                             int idx0, int idx1, int idx2, Vec3 *n);
static bool getNormalsComputed(Vec3 *v, Vec3 *n);

VuObject VuObject::Convert(const tinyobj::attrib_t &attrib,
                           const tinyobj::shape_t &shape,
                           const std::vector<tinyobj::material_t> &objMaterials,
                           const std::string &baseDir)
{
    VuObject o;

    // Check for smoothing group and compute smoothing normals
    std::unordered_map<int, Vec3> smoothVertexNormals;
    if (hasSmoothingGroup(shape))
        computeSmoothingNormals(attrib, shape, smoothVertexNormals);

    // Process faces
    for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
        tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
        tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
        tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

        Vec3 v[3];
        getVertices(attrib, idx0.vertex_index, idx1.vertex_index, idx2.vertex_index, o.boundsMin, o.boundsMax, v);

        Vec2 tc[3];
        getTexCoords(attrib, idx0.texcoord_index, idx1.texcoord_index, idx2.texcoord_index, tc);

        Vec3 n[3];
        bool valid = getNormalsAttrib(attrib, idx0.normal_index, idx1.normal_index, idx2.normal_index, n);
        if (!valid && !smoothVertexNormals.empty())
            valid = getNormalsSmooth(smoothVertexNormals, idx0.normal_index, idx1.normal_index, idx2.normal_index, n);
        if (!valid)
            getNormalsComputed(v, n);

        Vec3 diffuse;
        getDiffuse(objMaterials, shape, f, diffuse);

        Vec3 c[3];
        for (int k = 0; k < 3; k++) {
            // Combine normal and diffuse to get color.
            float nFactor = 0.5;
            float dFactor = 1 - nFactor;
            c[k] = n[k] * nFactor + diffuse * dFactor;
            float len2 = c[k].Length2();
            if (len2 > 0.0f) {
                float len = sqrtf(len2);
                c[k] = c[k] / len * 0.5f + 0.5f;
            }
        }

        for (int k = 0; k < 3; k++) {
            o.indices.emplace_back(o.buffer.size());
            o.buffer.emplace_back(v[k], n[k], c[k], tc[k]);
        }
    }

    o.name = shape.name;

    o.mtl = &Material::Load(objMaterials[shape.mesh.material_ids[0]]);

    return o;
}

bool getNormalsComputed(Vec3 *v, Vec3 *n)
{
    n[0] = Vec3::Normal(v[0], v[1], v[2]);
    n[1] = n[0];
    n[2] = n[0];
    return true;
}

bool getNormalsSmooth(std::unordered_map<int, Vec3> &smoothVertexNormals,
                      int idx0, int idx1, int idx2, Vec3 *n)
{
    if (idx0 >= 0 && idx1 >= 0 && idx2 >= 0) {
        n[0] = smoothVertexNormals[idx0];
        n[1] = smoothVertexNormals[idx1];
        n[2] = smoothVertexNormals[idx2];
        return true;
    }

    return false;
}

bool getNormalsAttrib(const tinyobj::attrib_t &attrib, int idx0, int idx1, int idx2, Vec3 *n)
{
    if (attrib.normals.empty())
        return false;

    if ((idx0 < 0) || (idx1 < 0) || (idx2 < 0))
        return false;

    for (int k = 0; k < 3; k++) {
        assert(size_t(3 * idx0 + k) < attrib.normals.size());
        assert(size_t(3 * idx1 + k) < attrib.normals.size());
        assert(size_t(3 * idx2 + k) < attrib.normals.size());
        n[0][k] = attrib.normals[3 * idx0 + k];
        n[1][k] = attrib.normals[3 * idx1 + k];
        n[2][k] = attrib.normals[3 * idx2 + k];
    }
    return true;
}

void getDiffuse(const std::vector<tinyobj::material_t> &obj_materials,
                const tinyobj::shape_t &current_shape,
                size_t f,
                Vec3 &diffuse)
{
    int currentMaterialId = current_shape.mesh.material_ids[f];
    if ((currentMaterialId < 0) || (currentMaterialId >= obj_materials.size()))
        currentMaterialId = obj_materials.size() - 1;

    for (size_t i = 0; i < 3; i++)
        diffuse[i] = obj_materials[currentMaterialId].diffuse[i];
}

void getTexCoords(const tinyobj::attrib_t &attrib, int idx0, int idx1, int idx2, Vec2 *tc)
{
    if (!attrib.texcoords.empty()) {
        if ((idx0 < 0) || (idx1 < 0) || (idx2 < 0)) {
            tc[0] = {0.0f, 0.0f};
            tc[1] = {0.0f, 0.0f};
            tc[2] = {0.0f, 0.0f};
        } else {
            assert(attrib.texcoords.size() > size_t(2 * idx0 + 1));
            assert(attrib.texcoords.size() > size_t(2 * idx1 + 1));
            assert(attrib.texcoords.size() > size_t(2 * idx2 + 1));

            // Flip Y coord.
            tc[0] = {attrib.texcoords[2 * idx0], 1.0f - attrib.texcoords[2 * idx0 + 1]};
            tc[1] = {attrib.texcoords[2 * idx1], 1.0f - attrib.texcoords[2 * idx1 + 1]};
            tc[2] = {attrib.texcoords[2 * idx2], 1.0f - attrib.texcoords[2 * idx2 + 1]};
        }
    } else {
        tc[0] = {0.0f, 0.0f};
        tc[1] = {0.0f, 0.0f};
        tc[2] = {0.0f, 0.0f};
    }
}

void getVertices(const tinyobj::attrib_t &attrib,
                 int idx0, int idx1, int idx2,
                 Vec3 &boundsMin, Vec3 &boundsMax, Vec3 *v)
{
    assert(idx0 >= 0);
    assert(idx1 >= 0);
    assert(idx2 >= 0);

    for (int k = 0; k < 3; k++) {
        v[0][k] = attrib.vertices[3 * idx0 + k];
        v[1][k] = attrib.vertices[3 * idx1 + k];
        v[2][k] = attrib.vertices[3 * idx2 + k];

        boundsMin[k] = std::min(v[0][k], boundsMin[k]);
        boundsMin[k] = std::min(v[1][k], boundsMin[k]);
        boundsMin[k] = std::min(v[2][k], boundsMin[k]);
        boundsMax[k] = std::max(v[0][k], boundsMax[k]);
        boundsMax[k] = std::max(v[1][k], boundsMax[k]);
        boundsMax[k] = std::max(v[2][k], boundsMax[k]);
    }
}

bool hasSmoothingGroup(const tinyobj::shape_t &shape)
{
    for (unsigned int id : shape.mesh.smoothing_group_ids)
        if (id > 0)
            return true;

    return false;
}

void computeSmoothingNormals(const tinyobj::attrib_t &attrib,
                             const tinyobj::shape_t &shape,
                             std::unordered_map<int, Vec3> &smoothVertexNormals)
{
    smoothVertexNormals.clear();

    // All faces are triangular.
    for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
        // Get the three indices of the face
        tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
        tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
        tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

        // Get the three vertex indices and coordinates
        int index[3];
        Vec3 coord[3];
        for (int k = 0; k < 3; k++) {
            index[0] = idx0.vertex_index;
            index[1] = idx1.vertex_index;
            index[2] = idx2.vertex_index;
            assert(index[0] >= 0);
            assert(index[1] >= 0);
            assert(index[2] >= 0);
            coord[0][k] = attrib.vertices[3 * index[0] + k];
            coord[1][k] = attrib.vertices[3 * index[1] + k];
            coord[2][k] = attrib.vertices[3 * index[2] + k];
        }

        // Compute the normal of the face
        Vec3 normal = Vec3::Normal(coord[0], coord[1], coord[2]);

        // Add the normal to the three vertices
        for (int i : index) {
            auto iter = smoothVertexNormals.find(i);
            if (iter != smoothVertexNormals.end()) {
                iter->second.x += normal.x;
                iter->second.y += normal.y;
                iter->second.z += normal.z;
            } else {
                smoothVertexNormals[i].x = normal.x;
                smoothVertexNormals[i].y = normal.y;
                smoothVertexNormals[i].z = normal.z;
            }
        }
    }

    // Normals should be unit vectors
    for (auto &n : smoothVertexNormals)
        n.second.Normalize();
}
