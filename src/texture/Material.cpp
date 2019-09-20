#include "Material.h"

#include <utility>
#include <iostream>

std::unordered_map<std::string, Material> Material::sCache;

Material::Material(std::string name)
    : name(std::move(name))
{
}

Material::Material(const tinyobj::material_t &material)
    : name(material.name)
{
    SetDiffuse(material.diffuse_texname);
    SetNormalMap(material.normal_texname);
}

void Material::SetDiffuse(const std::string &filename) {
    diffuseTexname = !filename.empty()
        ? "data/textures/" + filename
        : "data/textures/white.png";

    diffuseTexture = VuTexture::Load(diffuseTexname);
}

void Material::SetNormalMap(const std::string &filename) {
    normalTexname = !filename.empty()
        ? "data/textures/" + filename
        : "data/textures/normal.png";

    normalTexture = VuTexture::Load(normalTexname);
}

void Material::Bind() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture.id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalTexture.id);
}

void Material::Unbind() const
{

}

Material &Material::Load(const tinyobj::material_t &material)
{
    auto textureIterator = sCache.find(material.name);
    if (textureIterator == sCache.end()) {
        auto mat = sCache.emplace(std::make_pair(material.name, Material(material)));
        return mat.first->second;
    }

    return textureIterator->second;
}


