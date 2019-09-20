#pragma once

#include "common.h"

#include <unordered_map>
#include <string>

struct VuTexture
{
    unsigned int id = 0;
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char *buffer = nullptr;

    VuTexture() = default;
    VuTexture(int width, int height, int comp, unsigned char *bytes);

    void Bind() const;
    void Unbind() const;

    static VuTexture Load(const std::string &texturePath);

private:
    static std::unordered_map<std::string, VuTexture> sCache;
};

