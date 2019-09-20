#pragma once

#include <unordered_map>
#include <memory>
#include <math/Mat.h>
#include "ShaderGL.h"

class ShaderLibrary
{
public:
    void Add(const std::shared_ptr<ShaderGL>& shader);
    void Add(const std::string &name, const std::shared_ptr<ShaderGL> &shader);
    std::shared_ptr<ShaderGL> Load(const std::string& filePath);
    std::shared_ptr<ShaderGL> Load(const std::string& name, const std::string& filePath);
    std::shared_ptr<ShaderGL> Get(const std::string& name);

private:
    std::unordered_map<std::string, std::shared_ptr<ShaderGL>> shaders;
    bool Exists(const std::string &name) const;
};

