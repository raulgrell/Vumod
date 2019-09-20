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

static const char *SHADER_FILE = "data/shaders/scene.glsl";

Scene::Scene() : vs(SHADER_FILE)
{
    vc.position = {0, 10, 0};
    vc.rotation = {30, 0, 0};
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
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    std::string baseDir = file::baseDir(filename);

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials,
                                &warn, &err,
                                filename, baseDir.c_str());

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
              << " materials=" << materials.size()
              << " shapes=" << shapes.size()
              << " />" << std::endl;

    for (auto &m : materials) {
        auto mtl = Material::Load(m);
        std::cout << "<Material " << mtl.name
                  << " diffuse=" << mtl.diffuseTexname
                  << " normal=" << mtl.normalTexname
                  << " />" << std::endl;
    }

    // Append default material
    materials.emplace_back();

    boundsMin[0] = boundsMin[1] = boundsMin[2] = std::numeric_limits<float>::max();
    boundsMax[0] = boundsMax[1] = boundsMax[2] = -std::numeric_limits<float>::max();

    for (size_t s = 0; s < shapes.size(); s++) {
        auto o = VuObject::Convert(attrib, shapes[s], materials, baseDir);

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

            o.vbo = std::make_shared<VertexBuffer>(&o.buffer.at(0), o.buffer.size() * layout.GetStride());
            o.vbo->SetLayout(layout);

            o.ibo = std::make_shared<IndexBuffer>(&o.indices.at(0), o.indices.size());

            vs.vao->AddVertexBuffer(o.vbo);
        }
        objects.push_back(o);
    }

    CHECK_GL();

    return true;
}

void Scene::Prepare()
{

}

void Scene::Update()
{

}

void Scene::Render()
{
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    for (auto &object : objects) {
        vs.Bind();
        object.vbo->Bind();
        object.ibo->Bind();
        object.mtl->Bind();

        vs.LoadMatrix(vs.uniformMvp, vc.GetViewMatrix());
        vs.LoadVector(vs.uniformTint, {1, 1, 1, 1});

        glDrawElements(GL_TRIANGLES, object.indices.size(), GL_UNSIGNED_INT, nullptr);

        object.mtl->Unbind();
        object.ibo->Unbind();
        object.vbo->Unbind();
    }

    vs.Unbind();
    CHECK_GL();
}
