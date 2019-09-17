#pragma once

#include <unordered_map>
#include <memory>
#include <math/Mat.h>
#include <api/IShader.h>

class ShaderLibrary
{
public:
    void Add(const std::shared_ptr<IShader>& shader);
    std::shared_ptr<IShader> Load(const std::string& file_path);
    std::shared_ptr<IShader> Load(const std::string& name, const std::string& file_path);
    std::shared_ptr<IShader> Get(const std::string& name);

private:
    std::unordered_map<std::string, std::shared_ptr<IShader>> shaders;
};

