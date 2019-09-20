#pragma once

#include "VuTexture.h"
#include "scene/SceneShader.h"

#include <tinyobjloader/tiny_obj_loader.h>

#include <unordered_map>
#include <string>

struct Material
{
    std::string name;
    std::string diffuseTexname;
    std::string normalTexname;

    VuTexture diffuseTexture;
    VuTexture normalTexture;

    Material() = default;
    explicit Material(std::string name);
    explicit Material(const tinyobj::material_t &material);

    void Bind() const;
    void Unbind() const;

    void SetDiffuse(const std::string &filename);
    void SetNormalMap(const std::string &filename);

    static Material &Load(const tinyobj::material_t &param);

private:
    static std::unordered_map<std::string, Material> sCache;
};
