#include "Scene.h"
#include "files/VFS.h"
#include "models/VuObject.h"
#include "texture/VuTexture.h"

#include <glad/glad.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <files/FileUtils.h>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

static const char *VERTEX_FILE = "data/shaders/texture_vert.glsl";
static const char *FRAGMENT_FILE = "data/shaders/texture_frag.glsl";

Scene::Scene()
        : vs("Scene", VERTEX_FILE, FRAGMENT_FILE), bounds_max(), bounds_min()
{
    vc.position = {0, 10, 0};
    vc.rotation = {M_PI / 4, 0, 0};
}

void Scene::Update(VuWindow &vw)
{
    vw.GetSize(&width, &height);
    glViewport(0, 0, width, height);
}

void Scene::Draw()
{
    vs.Bind();
    CheckGL();

    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    for (auto &object : objects) {
        object.vbo->Bind();
        object.ibo->Bind();
        object.mtl->Bind();

        vs.LoadMatrix(vs.uniform_mvp, vc.GetViewMatrix());
        vs.LoadVector(vs.uniform_tint, {1, 1, 1, 1});

        glDrawElements(GL_TRIANGLES, object.indices.size(), GL_UNSIGNED_INT, nullptr);

        object.mtl->Unbind();
        object.ibo->Unbind();
        object.vbo->Unbind();
    }


    vs.Unbind();
    CheckGL();
}

void Scene::LoadFile(const char *path)
{
    const char *filename = path;
    if (!LoadObject(filename))
        fatal_error("Could not load model.");
}

bool Scene::LoadObject(const char *filename)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> obj_materials;
    std::string warn, err;
    std::string base_dir = GetBaseDir(filename);

    bool ret = tinyobj::LoadObj(
            &attrib, &shapes, &obj_materials,
            &warn, &err,
            filename, base_dir.c_str());

    if (!warn.empty())
        std::cout << "WARN: " << warn << std::endl;

    if (!err.empty())
        std::cerr << err << std::endl;

    if (!ret) {
        std::cerr << "Failed to load " << filename << std::endl;
        return false;
    }

    std::cout << "<File "
              << filename
              << " vertices=" << attrib.vertices.size() / 3
              << " normals=" << attrib.normals.size() / 3
              << " texcoords=" << attrib.texcoords.size() / 2
              << " materials=" << obj_materials.size()
              << " shapes=" << shapes.size()
              << " />" << std::endl;;

    for (auto &m : obj_materials) {
        auto mtl = materials.emplace_back(vs, m, base_dir);
        std::cout << "<Material[" << materials.size() - 1 << "] "
                  << mtl.name
                  << " diffuse=" << mtl.diffuse_texname
                  << " normal=" << mtl.normal_texname
                  << " />" << std::endl;
    }

    // Append default material
    obj_materials.emplace_back();

    bounds_min[0] = bounds_min[1] = bounds_min[2] = std::numeric_limits<float>::max();
    bounds_max[0] = bounds_max[1] = bounds_max[2] = -std::numeric_limits<float>::max();

    for (size_t s = 0; s < shapes.size(); s++) {
        auto o = Convert(attrib, shapes[s], obj_materials, base_dir);

        std::cout << "<Shape[" << s << "] "
                  << shapes[s].name
                  << " triangles=" << o.NumTriangles()
                  << " />" << std::endl;

        if (!o.buffer.empty()) {
            BufferLayout layout {
                    {"vPos",  GL_FLOAT, 3},
                    {"vCol",  GL_FLOAT, 3},
                    {"vNorm", GL_FLOAT, 3},
                    {"vUV",   GL_FLOAT, 2}};

            o.vbo = std::make_shared<VertexBuffer>(
                    &o.buffer.at(0),
                    o.buffer.size() * layout.GetStride());
            o.vbo->SetLayout(layout);

            o.ibo = std::make_shared<IndexBuffer>(&o.indices.at(0), o.indices.size());

            vs.vao->AddVertexBuffer(o.vbo);
        }
        objects.push_back(o);
    }

    return true;
}

VuObject Scene::Convert(const tinyobj::attrib_t &attrib,
                        const tinyobj::shape_t &shape,
                        const std::vector<tinyobj::material_t> &obj_materials,
                        const std::string &base_dir)
{
    VuObject o;

    // Check for smoothing group and compute smoothing normals
    std::unordered_map<int, Vec3> smoothVertexNormals;
    if (hasSmoothingGroup(shape))
        computeSmoothingNormals(attrib, shape, smoothVertexNormals);

    for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
        tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
        tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
        tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

        Vec3 diffuse;
        GetDiffuse(obj_materials, shape, f, diffuse);

        Vec2 tc[3];
        GetTexCoords(attrib, idx0, idx1, idx2, tc);

        Vec3 v[3];
        GetVertices(attrib, idx0, idx1, idx2, v);

        Vec3 n[3];
        bool valid = GetNormalsAttrib(attrib, idx0, idx1, idx2, n);
        if (!valid && !smoothVertexNormals.empty())
            valid = GetNormalsSmooth(smoothVertexNormals, idx0, idx1, idx2, n);
        if (!valid)
            GetNormalsComputed(v, n);

        for (int k = 0; k < 3; k++) {
            // Combine normal and diffuse to get color.
            float n_factor = 0.5;
            float d_factor = 1 - n_factor;
            Vec3 c = {n[k][0] * n_factor + diffuse[0] * d_factor,
                      n[k][1] * n_factor + diffuse[1] * d_factor,
                      n[k][2] * n_factor + diffuse[2] * d_factor};
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
    o.mtl = !shape.mesh.material_ids.empty()
            ? &materials[0]
            : &materials[0];

    return o;
}


bool Scene::GetNormalsComputed(Vec3 *v, Vec3 *n) const
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

bool Scene::GetNormalsSmooth(std::unordered_map<int, Vec3> &smoothVertexNormals,
                             const tinyobj::index_t &idx0,
                             const tinyobj::index_t &idx1,
                             const tinyobj::index_t &idx2,
                             Vec3 *n) const
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

bool Scene::GetNormalsAttrib(const tinyobj::attrib_t &attrib,
                             const tinyobj::index_t &idx0,
                             const tinyobj::index_t &idx1,
                             const tinyobj::index_t &idx2,
                             Vec3 *n) const
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

void Scene::GetDiffuse(const std::vector<tinyobj::material_t> &obj_materials,
                       const tinyobj::shape_t &current_shape,
                       size_t f,
                       Vec3 &diffuse) const
{
    int current_material_id = current_shape.mesh.material_ids[f];
    if ((current_material_id < 0) || (current_material_id >= obj_materials.size()))
        current_material_id = obj_materials.size() - 1;

    for (size_t i = 0; i < 3; i++)
        diffuse[i] = obj_materials[current_material_id].diffuse[i];
}

void Scene::GetTexCoords(const tinyobj::attrib_t &attrib,
                         const tinyobj::index_t &idx0,
                         const tinyobj::index_t &idx1,
                         const tinyobj::index_t &idx2,
                         Vec2 *tc) const
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

void Scene::GetVertices(const tinyobj::attrib_t &attrib,
                        const tinyobj::index_t &idx0,
                        const tinyobj::index_t &idx1,
                        const tinyobj::index_t &idx2,
                        Vec3 *v)
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