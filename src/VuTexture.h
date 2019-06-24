#pragma once

#include "common.h"

struct VuTexture {
    GLuint id = 0;
    GLsizei width = 0, height = 0;
    std::string path;

    static std::unordered_map<std::string, VuTexture> s_Cache;

    static VuTexture Load(const std::string &texturePath);

    void Bind() const;

    void Unbind() const;
};

