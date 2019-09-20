#include "VuTexture.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <iostream>
#include <cassert>

std::unordered_map<std::string, VuTexture> VuTexture::sCache;

static VuTexture LoadPng(const std::string &filePath)
{
    int width, height, comp;
    stbi_set_flip_vertically_on_load(true);
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
    auto textureIterator = sCache.find(texturePath);
    if (textureIterator == sCache.end()) {
        VuTexture newTexture = LoadPng(texturePath);
        sCache.insert(std::make_pair(texturePath, newTexture));
        std::cout << "<Texture "
            << texturePath
            << " width=" << newTexture.width
            << " height=" << newTexture.height
            << " channels=" << newTexture.channels
            << " />" <<std::endl;
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

VuTexture::VuTexture(int width, int height, int comp, unsigned char *bytes)
    : width(width), height(height), channels(comp)
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

