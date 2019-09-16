#include "VuObject.h"

#include <cassert>

void CalcNormal(Vec3 N, Vec3 &v0, Vec3 &v1, Vec3 &v2)
{
    Vec3 v10;
    v10[0] = v1[0] - v0[0];
    v10[1] = v1[1] - v0[1];
    v10[2] = v1[2] - v0[2];

    Vec3 v20;
    v20[0] = v2[0] - v0[0];
    v20[1] = v2[1] - v0[1];
    v20[2] = v2[2] - v0[2];

    N[0] = v20[1] * v10[2] - v20[2] * v10[1];
    N[1] = v20[2] * v10[0] - v20[0] * v10[2];
    N[2] = v20[0] * v10[1] - v20[1] * v10[0];

    float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
    if (len2 > 0.0f) {
        float len = sqrtf(len2);
        N[0] /= len;
        N[1] /= len;
        N[2] /= len;
    }
}

// Check if `mesh_t` contains smoothing group id.
bool hasSmoothingGroup(const tinyobj::shape_t &shape)
{
    for (unsigned int smoothing_group_id : shape.mesh.smoothing_group_ids)
        if (smoothing_group_id > 0)
            return true;

    return false;
}

void computeSmoothingNormals(const tinyobj::attrib_t &attrib,
                             const tinyobj::shape_t &shape,
                             std::unordered_map<int, Vec3> &smoothVertexNormals)
{
    smoothVertexNormals.clear();

    for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
        // Get the three indexes of the face (all faces are triangular)
        tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
        tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
        tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

        // Get the three vertex indexes and coordinates
        int vi[3];     // indexes
        Vec3 v[3]; // coordinates
        for (int k = 0; k < 3; k++) {
            vi[0] = idx0.vertex_index;
            vi[1] = idx1.vertex_index;
            vi[2] = idx2.vertex_index;
            assert(vi[0] >= 0);
            assert(vi[1] >= 0);
            assert(vi[2] >= 0);

            v[0][k] = attrib.vertices[3 * vi[0] + k];
            v[1][k] = attrib.vertices[3 * vi[1] + k];
            v[2][k] = attrib.vertices[3 * vi[2] + k];
        }

        // Compute the normal of the face
        Vec3 normal;
        CalcNormal(normal, v[0], v[1], v[2]);

        // Add the normal to the three vertexes
        for (int i : vi) {
            auto iter = smoothVertexNormals.find(i);
            if (iter != smoothVertexNormals.end()) {
                // add
                iter->second[0] += normal[0];
                iter->second[1] += normal[1];
                iter->second[2] += normal[2];
            } else {
                smoothVertexNormals[i][0] = normal[0];
                smoothVertexNormals[i][1] = normal[1];
                smoothVertexNormals[i][2] = normal[2];
            }
        }
    }

    // Normals should be unit vectors
    for (auto &n : smoothVertexNormals)
        n.second.Normalize();
}