#pragma once

#include <tinyobjloader/tiny_obj_loader.h>
#include "scene/SceneShader.h"
#include "VuTexture.h"

struct VuMaterial
{
    std::string name;
    std::string diffuse_texname;
    std::string normal_texname;

    const SceneShader& shader;
    VuTexture diffuse_texture;
    VuTexture normal_texture;

    explicit VuMaterial(const SceneShader &shader);
    VuMaterial(const SceneShader &shader, const tinyobj::material_t &material, const std::string &base_dir);

    void Bind() const;
    void Unbind() const;
};
