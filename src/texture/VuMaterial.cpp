
#include "VuMaterial.h"

VuMaterial::VuMaterial(const SceneShader &shader)
    : shader(shader)
{
}

VuMaterial::VuMaterial(const SceneShader &shader, const tinyobj::material_t &material, const std::string &base_dir)
    : shader(shader), name(material.name)
{
    diffuse_texname = !material.diffuse_texname.empty()
        ? "data/textures/" + material.diffuse_texname
        : "data/textures/white.png";

    diffuse_texture = VuTexture::Load(diffuse_texname);

    normal_texname = !material.diffuse_texname.empty()
        ? "data/textures/" + material.diffuse_texname
        : "data/textures/normal.png";

    normal_texture = VuTexture::Load(normal_texname);
}

void VuMaterial::Bind() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture.id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal_texture.id);
}

void VuMaterial::Unbind() const
{
}


