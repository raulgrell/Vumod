#include "VuScene.h"
#include "VuCamera.h"
#include "VuFS.h"
#include "VuObject.h"
#include "VuTexture.h"
#include <graphics/VertexArray.h>

#include <glad/glad.h>

#include <algorithm>
#include <cassert>
#include <iostream>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

VuScene::VuScene(VuWindow &vw, VuShader &vs)
        : vw(vw), vs(vs), bounds_max(), bounds_min()
{
    vc.position[0] = 0;
    vc.position[1] = 0.5;
    vc.position[2] = -16;
}

void VuScene::UpdateCamera()
{
    vw.GetSize(&width, &height);
    glViewport(0, 0, width, height);
    vc.Update(width, height);
}

void VuScene::Draw()
{
    vs.Bind();
    CheckErrors();

    for (auto &object : objects) {
        glBindBuffer(GL_ARRAY_BUFFER, object.vbo_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.ibo_id);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, object.texture_id);

        glUniformMatrix4fv(vs.uniform_mvp, 1, GL_FALSE, (const GLfloat *) vc.mvp);
        glUniform4fv(vs.uniform_tint, 1, (const GLfloat *) vec4 {0, 0, 0, 0});

        glVertexAttribPointer(vs.attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(VuVertex),
                              (void *) offsetof(VuVertex, position));
        glVertexAttribPointer(vs.attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(VuVertex),
                              (void *) offsetof(VuVertex, normal));
        glVertexAttribPointer(vs.attr_color, 3, GL_FLOAT, GL_FALSE, sizeof(VuVertex),
                              (void *) offsetof(VuVertex, color));
        glVertexAttribPointer(vs.attr_uv, 2, GL_FLOAT, GL_FALSE, sizeof(VuVertex), (void *) offsetof(VuVertex, uv));

        glDrawElements(GL_TRIANGLES, object.indices.size(), GL_UNSIGNED_INT, (void*)0);
    }

    CheckErrors();
    vs.Unbind();
}

void VuScene::LoadFile(const char *path)
{
    const char *filename = path;
    size_t filesize = GetFileSize(filename);
    printf("Loading %s, size %lluB\n", filename, filesize);

    if (!LoadObject(filename))
        fatal_error("Could not load model.");
}

bool VuScene::LoadObject(const char *filename)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> obj_materials;
    std::string warn, err;
    std::string base_dir = GetBaseDir(filename);

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &obj_materials, &warn, &err, filename, base_dir.c_str());

    if (!warn.empty())
        std::cout << "WARN: " << warn << std::endl;

    if (!err.empty())
        std::cerr << err << std::endl;

    if (!ret) {
        std::cerr << "Failed to load " << filename << std::endl;
        return false;
    }

    std::cout << "# of vertices  = " << attrib.vertices.size() / 3 << std::endl;
    std::cout << "# of normals   = " << attrib.normals.size() / 3 << std::endl;
    std::cout << "# of texcoords = " << attrib.texcoords.size() / 2 << std::endl;
    std::cout << "# of materials = " << obj_materials.size() << std::endl;
    std::cout << "# of shapes    = " << shapes.size() << std::endl;

    for (size_t i = 0; i < obj_materials.size(); i++) {
        auto &m = obj_materials[i];
        std::cout << "material[" << i << "]<" << m.name << " diffuse='" << m.diffuse_texname << "'>" << std::endl;
    }

    // Append default material
    obj_materials.emplace_back();

    for (auto &material : obj_materials) {
        if (!material.diffuse_texname.empty()) {
            std::string texture_filename = base_dir + "/" + material.diffuse_texname;
            VuTexture texture = VuTexture::Load(texture_filename);
        }
    }

    bounds_min[0] = bounds_min[1] = bounds_min[2] = std::numeric_limits<float>::max();
    bounds_max[0] = bounds_max[1] = bounds_max[2] = -std::numeric_limits<float>::max();

    for (size_t s = 0; s < shapes.size(); s++) {
        auto o = Convert(attrib, shapes[s], obj_materials, base_dir);

        std::cout << "shape[" << s << "]<"
                  << shapes[s].name
                  << " material_id=" << o.material_id
                  << " triangles=" << o.NumTriangles()
                  << " />" << std::endl;

        if (!o.buffer.empty()) {

            glBindVertexArray(vs.vao_id);

            glGenBuffers(1, &o.vbo_id);
            glBindBuffer(GL_ARRAY_BUFFER, o.vbo_id);
            glBufferData(GL_ARRAY_BUFFER, o.buffer.size() * sizeof(VuVertex), &o.buffer.at(0), GL_STATIC_DRAW);

            glGenBuffers(1, &o.ibo_id);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o.ibo_id);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, o.indices.size() * sizeof(unsigned int), &o.indices.at(0), GL_STATIC_DRAW);
        }
        objects.push_back(o);
    }

    return true;
}

VuObject VuScene::Convert(const tinyobj::attrib_t &attrib,
                          const tinyobj::shape_t &shape,
                          const std::vector<tinyobj::material_t> &obj_materials,
                          const std::string &base_dir)
{
    VuObject o;

    // Check for smoothing group and compute smoothing normals
    std::unordered_map<int, vec3> smoothVertexNormals;
    if (hasSmoothingGroup(shape))
        computeSmoothingNormals(attrib, shape, smoothVertexNormals);

    for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
        tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
        tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
        tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

        vec3 diffuse;
        GetDiffuse(obj_materials, shape, f, diffuse);

        vec2 tc[3];
        GetTexCoords(attrib, idx0, idx1, idx2, tc);

        vec3 v[3];
        GetVertices(attrib, idx0, idx1, idx2, v);

        vec3 n[3];
        bool valid = GetNormalsAttrib(attrib, idx0, idx1, idx2, n);
        if (!valid && !smoothVertexNormals.empty())
            valid = GetNormalsSmooth(smoothVertexNormals, idx0, idx1, idx2, n);
        if (!valid)
            GetNormalsComputed(v, n);

        for (int k = 0; k < 3; k++) {
            // Combine normal and diffuse to get color.
            float normal_factor = 0.5;
            float diffuse_factor = 1 - normal_factor;
            float c[3] = {n[k][0] * normal_factor + diffuse[0] * diffuse_factor,
                          n[k][1] * normal_factor + diffuse[1] * diffuse_factor,
                          n[k][2] * normal_factor + diffuse[2] * diffuse_factor};
            float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
            if (len2 > 0.0f) {
                float len = sqrtf(len2);
                c[0] = c[0] / len * 0.5f + 0.5f;
                c[1] = c[1] / len * 0.5f + 0.5f;
                c[2] = c[2] / len * 0.5f + 0.5f;
            }

            o.indices.emplace_back(o.buffer.size());
            o.buffer.emplace_back(v[k], n[k], c, tc[k]);
        }
    }

    o.name = shape.name;
    o.material_id = !shape.mesh.material_ids.empty() ? shape.mesh.material_ids[0] : obj_materials.size() - 1;

    const std::string &texname = obj_materials[o.material_id].diffuse_texname;
    std::string texture_filename = !texname.empty() ? base_dir + "/" + texname : base_dir + "/white.png";

    VuTexture texture = VuTexture::Load(texture_filename);
    o.texture_id = texture.id;

    return o;
}


bool VuScene::GetNormalsComputed(vec3 const *v, vec3 *n) const
{
    CalcNormal(n[0], v[0], v[1], v[2]);
    n[1][0] = n[0][0];
    n[1][1] = n[0][1];
    n[1][2] = n[0][2];
    n[2][0] = n[0][0];
    n[2][1] = n[0][1];
    n[2][2] = n[0][2];
    return true;
}

bool VuScene::GetNormalsSmooth(std::unordered_map<int, vec3> &smoothVertexNormals,
                               const tinyobj::index_t &idx0,
                               const tinyobj::index_t &idx1,
                               const tinyobj::index_t &idx2,
                               vec3 *n) const
{
    int f0 = idx0.vertex_index;
    int f1 = idx1.vertex_index;
    int f2 = idx2.vertex_index;

    if (f0 >= 0 && f1 >= 0 && f2 >= 0) {
        n[0][0] = smoothVertexNormals[f0][0];
        n[0][1] = smoothVertexNormals[f0][1];
        n[0][2] = smoothVertexNormals[f0][2];

        n[1][0] = smoothVertexNormals[f1][0];
        n[1][1] = smoothVertexNormals[f1][1];
        n[1][2] = smoothVertexNormals[f1][2];

        n[2][0] = smoothVertexNormals[f2][0];
        n[2][1] = smoothVertexNormals[f2][1];
        n[2][2] = smoothVertexNormals[f2][2];

        return true;
    }

    return false;
}

bool VuScene::GetNormalsAttrib(const tinyobj::attrib_t &attrib,
                               const tinyobj::index_t &idx0,
                               const tinyobj::index_t &idx1,
                               const tinyobj::index_t &idx2, vec3 *n) const
{
    if (attrib.normals.empty())
        return false;

    int nf0 = idx0.normal_index;
    int nf1 = idx1.normal_index;
    int nf2 = idx2.normal_index;

    if ((nf0 < 0) || (nf1 < 0) || (nf2 < 0))
        return false;

    for (int k = 0; k < 3; k++) {
        assert(size_t(3 * nf0 + k) < attrib.normals.size());
        assert(size_t(3 * nf1 + k) < attrib.normals.size());
        assert(size_t(3 * nf2 + k) < attrib.normals.size());
        n[0][k] = attrib.normals[3 * nf0 + k];
        n[1][k] = attrib.normals[3 * nf1 + k];
        n[2][k] = attrib.normals[3 * nf2 + k];
    }
    return true;
}

void VuScene::GetDiffuse(const std::vector<tinyobj::material_t> &obj_materials,
                         const tinyobj::shape_t &current_shape,
                         size_t f,
                         vec3 diffuse) const
{
    int current_material_id = current_shape.mesh.material_ids[f];
    if ((current_material_id < 0) || (current_material_id >= obj_materials.size()))
        current_material_id = obj_materials.size() - 1;

    for (size_t i = 0; i < 3; i++)
        diffuse[i] = obj_materials[current_material_id].diffuse[i];
}

void VuScene::GetTexCoords(const tinyobj::attrib_t &attrib,
                           const tinyobj::index_t &idx0,
                           const tinyobj::index_t &idx1,
                           const tinyobj::index_t &idx2,
                           vec2 *tc) const
{
    if (!attrib.texcoords.empty()) {
        if ((idx0.texcoord_index < 0) || (idx1.texcoord_index < 0) || (idx2.texcoord_index < 0)) {
            // face does not contain valid uv index.
            tc[0][0] = 0.0f;
            tc[0][1] = 0.0f;
            tc[1][0] = 0.0f;
            tc[1][1] = 0.0f;
            tc[2][0] = 0.0f;
            tc[2][1] = 0.0f;
        } else {
            assert(attrib.texcoords.size() > size_t(2 * idx0.texcoord_index + 1));
            assert(attrib.texcoords.size() > size_t(2 * idx1.texcoord_index + 1));
            assert(attrib.texcoords.size() > size_t(2 * idx2.texcoord_index + 1));

            // Flip Y coord.
            tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
            tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
            tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
            tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
            tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
            tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];
        }
    } else {
        tc[0][0] = 0.0f;
        tc[0][1] = 0.0f;
        tc[1][0] = 0.0f;
        tc[1][1] = 0.0f;
        tc[2][0] = 0.0f;
        tc[2][1] = 0.0f;
    }
}

void VuScene::GetVertices(const tinyobj::attrib_t &attrib,
                          const tinyobj::index_t &idx0,
                          const tinyobj::index_t &idx1,
                          const tinyobj::index_t &idx2,
                          vec3 *v)
{
    int f0 = idx0.vertex_index;
    int f1 = idx1.vertex_index;
    int f2 = idx2.vertex_index;
    assert(f0 >= 0);
    assert(f1 >= 0);
    assert(f2 >= 0);

    for (int k = 0; k < 3; k++) {
        v[0][k] = attrib.vertices[3 * f0 + k];
        v[1][k] = attrib.vertices[3 * f1 + k];
        v[2][k] = attrib.vertices[3 * f2 + k];

        bounds_min[k] = std::min(v[0][k], bounds_min[k]);
        bounds_min[k] = std::min(v[1][k], bounds_min[k]);
        bounds_min[k] = std::min(v[2][k], bounds_min[k]);
        bounds_max[k] = std::max(v[0][k], bounds_max[k]);
        bounds_max[k] = std::max(v[1][k], bounds_max[k]);
        bounds_max[k] = std::max(v[2][k], bounds_max[k]);
    }
}