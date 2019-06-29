#pragma once

#include "common.h"

#include <unordered_map>

struct VuTexture
{
    unsigned int id = 0;
    int width = 0, height = 0;

    VuTexture(int width, int height, int comp, unsigned char *bytes);

    void Bind() const;
    void Unbind() const;

    static std::unordered_map<std::string, VuTexture> s_Cache;
    static VuTexture Load(const std::string &texturePath);
};

