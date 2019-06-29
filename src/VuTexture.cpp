#include "VuTexture.h"

#include <glad/glad.h>

#include <iostream>
#include <cassert>

std::unordered_map<std::string, VuTexture> VuTexture::s_Cache;

static VuTexture loadPng(const std::string &filePath)
{
    int width, height, comp;
    unsigned char *image = stbi_load(filePath.c_str(), &width, &height, &comp, STBI_default);
    if (!image) {
        std::cerr << "Unable to load image: " << filePath << std::endl;
        exit(1);
    }

    VuTexture texture(width, height, comp, image);
    texture.Unbind();

    stbi_image_free(image);

    return texture;
}

VuTexture VuTexture::Load(const std::string &texturePath)
{
    auto textureIterator = VuTexture::s_Cache.find(texturePath);
    if (textureIterator == VuTexture::s_Cache.end()) {
        VuTexture newTexture = loadPng(texturePath);
        VuTexture::s_Cache.insert(make_pair(texturePath, newTexture));
        return newTexture;
    }

    return textureIterator->second;
}

void VuTexture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void VuTexture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

VuTexture::VuTexture(int width, int height, int comp, unsigned char *bytes) : width(width), height(height)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    if (comp == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    else if (comp == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    else
        assert(0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
}

