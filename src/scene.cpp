#include "common.h"

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

VuScene::VuScene(VuWindow &vw, VuShader &vs) : vw(vw), vs(vs) {
    vc.position[0] = 0;
    vc.position[1] = 0.5;
    vc.position[2] = -16;
}

void VuScene::UpdateCamera() {
    glfwGetFramebufferSize(vw.window, &width, &height);
    glViewport(0, 0, width, height);
    vc.Update(width, height);
}

void VuScene::Draw() {
    vs.Bind();
    CheckErrors();

    for (auto &object : objects) {
        glBindBuffer(GL_ARRAY_BUFFER, object.vbo_id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, object.texture_id);

        glUniformMatrix4fv(vs.uniform_mvp, 1, GL_FALSE, (const GLfloat *) vc.mvp);
        glUniform4fv(vs.uniform_tint, 1, (const GLfloat *) vec4{0, 0, 0, 0});

        glVertexAttribPointer(vs.attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(VuVertex),
                              (void *) offsetof(VuVertex, position));
        glVertexAttribPointer(vs.attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(VuVertex),
                              (void *) offsetof(VuVertex, normal));
        glVertexAttribPointer(vs.attr_color, 3, GL_FLOAT, GL_FALSE, sizeof(VuVertex),
                              (void *) offsetof(VuVertex, color));
        glVertexAttribPointer(vs.attr_uv, 2, GL_FLOAT, GL_FALSE, sizeof(VuVertex), (void *) offsetof(VuVertex, uv));

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei) object.buffer.size());
    }

    CheckErrors();
    vs.Unbind();
}

static void CalcNormal(vec3 N, const vec3 v0, const vec3 v1, const vec3 v2) {
    vec3 v10;
    v10[0] = v1[0] - v0[0];
    v10[1] = v1[1] - v0[1];
    v10[2] = v1[2] - v0[2];

    vec3 v20;
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
bool hasSmoothingGroup(const tinyobj::shape_t &shape) {
    for (unsigned int smoothing_group_id : shape.mesh.smoothing_group_ids)
        if (smoothing_group_id > 0)
            return true;

    return false;
}

void computeSmoothingNormals(const tinyobj::attrib_t &attrib, const tinyobj::shape_t &shape,
                             std::unordered_map<int, vec3> &smoothVertexNormals) {
    smoothVertexNormals.clear();

    for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
        // Get the three indexes of the face (all faces are triangular)
        tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
        tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
        tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

        // Get the three vertex indexes and coordinates
        int vi[3];     // indexes
        vec3 v[3]; // coordinates
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
        float normal[3];
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
        vec3_norm(n.second, n.second);
}

void VuScene::LoadFile(const char *path) {
    const char *filename = path;
    size_t filesize = GetFileSize(filename);
    printf("Loading %s, size %lluB\n", filename, filesize);

    if (!LoadObject(filename))
        fatal_error("Could not load model.");
}

bool VuScene::LoadObject(const char *filename) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::unordered_map<std::string, int> textures = {};
    std::string warn, err;
    std::string base_dir = GetBaseDir(filename);
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename, base_dir.c_str());

    if (!warn.empty())
        std::cout << "WARN: " << warn << std::endl;

    if (!err.empty())
        std::cerr << err << std::endl;

    if (!ret) {
        std::cerr << "Failed to load " << filename << std::endl;
        return false;
    }

    printf("# of vertices  = %d\n", (int) (attrib.vertices.size()) / 3);
    printf("# of normals   = %d\n", (int) (attrib.normals.size()) / 3);
    printf("# of texcoords = %d\n", (int) (attrib.texcoords.size()) / 2);
    printf("# of materials = %d\n", (int) materials.size());
    printf("# of shapes    = %d\n", (int) shapes.size());

    for (size_t i = 0; i < materials.size(); i++)
        printf("material[%d].diffuse_texname = %s\n", int(i), materials[i].diffuse_texname.c_str());

    // Append default material
    materials.emplace_back();

    for (auto &material : materials) {
        if (!material.diffuse_texname.empty()) {
            std::string texture_filename = base_dir + "/" + material.diffuse_texname;
            VuTexture texture = VuTexture::Load(texture_filename);
        }
    }

    bounds_min[0] = bounds_min[1] = bounds_min[2] = std::numeric_limits<float>::max();
    bounds_max[0] = bounds_max[1] = bounds_max[2] = -std::numeric_limits<float>::max();

    for (size_t s = 0; s < shapes.size(); s++) {
        VuObject o;

        // Check for smoothing group and compute smoothing normals
        std::unordered_map<int, vec3> smoothVertexNormals;
        if (hasSmoothingGroup(shapes[s]) > 0) {
            std::cout << "Compute smoothingNormal for shape [" << shapes[s].name << "]" << std::endl;
            computeSmoothingNormals(attrib, shapes[s], smoothVertexNormals);
        }

        for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
            tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
            tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
            tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

            int current_material_id = shapes[s].mesh.material_ids[f];

            // Invaid material ID. Use default material (the last item in `materials`.)
            if ((current_material_id < 0) || (current_material_id >= static_cast<int>(materials.size())))
                current_material_id = materials.size() - 1;

            float diffuse[3];
            for (size_t i = 0; i < 3; i++)
                diffuse[i] = materials[current_material_id].diffuse[i];

            float tc[3][2];
            if (!attrib.texcoords.empty()) {
                if ((idx0.texcoord_index < 0) || (idx1.texcoord_index < 0) ||
                    (idx2.texcoord_index < 0)) {
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

            vec3 v[3];
            for (int k = 0; k < 3; k++) {
                int f0 = idx0.vertex_index;
                assert(f0 >= 0);
                int f1 = idx1.vertex_index;
                assert(f1 >= 0);
                int f2 = idx2.vertex_index;
                assert(f2 >= 0);

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

            float n[3][3];
            {
                bool invalid_normal_index = false;
                if (!attrib.normals.empty()) {
                    int nf0 = idx0.normal_index;
                    int nf1 = idx1.normal_index;
                    int nf2 = idx2.normal_index;

                    if ((nf0 < 0) || (nf1 < 0) || (nf2 < 0)) {
                        // normal index is missing from this face.
                        invalid_normal_index = true;
                    } else {
                        for (int k = 0; k < 3; k++) {
                            assert(size_t(3 * nf0 + k) < attrib.normals.size());
                            assert(size_t(3 * nf1 + k) < attrib.normals.size());
                            assert(size_t(3 * nf2 + k) < attrib.normals.size());
                            n[0][k] = attrib.normals[3 * nf0 + k];
                            n[1][k] = attrib.normals[3 * nf1 + k];
                            n[2][k] = attrib.normals[3 * nf2 + k];
                        }
                    }
                } else {
                    invalid_normal_index = true;
                }

                if (invalid_normal_index && !smoothVertexNormals.empty()) {
                    // Use smoothing normals
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

                        invalid_normal_index = false;
                    }
                }

                if (invalid_normal_index) {
                    // compute geometric normal
                    CalcNormal(n[0], v[0], v[1], v[2]);
                    n[1][0] = n[0][0];
                    n[1][1] = n[0][1];
                    n[1][2] = n[0][2];
                    n[2][0] = n[0][0];
                    n[2][1] = n[0][1];
                    n[2][2] = n[0][2];
                }
            }

            for (int k = 0; k < 3; k++) {
                // Combine normal and diffuse to get color.
                float normal_factor = 0.2;
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

                o.buffer.emplace_back(v[k], n[k], c, tc[k]);
            }
        }

        // OpenGL viewer does not support texturing with per-face material.
        if (!shapes[s].mesh.material_ids.empty() && shapes[s].mesh.material_ids.size() > s)
            o.material_id = shapes[s].mesh.material_ids[0]; // use the material ID of the first face.
        else
            o.material_id = materials.size() - 1; // = ID for default material.

        if (!materials[o.material_id].diffuse_texname.empty()) {
            std::string texture_filename = base_dir + "/" + materials[o.material_id].diffuse_texname;
            VuTexture texture = VuTexture::Load(texture_filename);
            o.texture_id = texture.id;
        } else {
            std::string texture_filename = base_dir + "/white.png";
            VuTexture texture = VuTexture::Load(texture_filename);
            o.texture_id = texture.id;
        }

        printf("shape[%d] material_id %d\n", int(s), int(o.material_id));

        if (!o.buffer.empty()) {
            glBindVertexArray(vs.vao_id);
            glGenBuffers(1, &o.vbo_id);
            glBindBuffer(GL_ARRAY_BUFFER, o.vbo_id);
            glBufferData(GL_ARRAY_BUFFER, o.buffer.size() * sizeof(VuVertex), &o.buffer.at(0), GL_STATIC_DRAW);

            printf("shape[%d] # of triangles = %d\n", static_cast<int>(s), o.NumTriangles());
        }

        objects.push_back(o);
    }

    return true;
}

