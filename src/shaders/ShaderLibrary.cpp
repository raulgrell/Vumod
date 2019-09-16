#include "ShaderLibrary.h"

void ShaderLibrary::Add(const std::shared_ptr<IShader> &shader)
{
    auto name = shader->GetName();
    shaders[name] = shader;
}

std::shared_ptr<IShader> ShaderLibrary::Load(const std::string &file_path)
{
    return std::shared_ptr<IShader>();
}

std::shared_ptr<IShader> ShaderLibrary::Load(const std::string &name, const std::string &file_path)
{
    return std::shared_ptr<IShader>();
}

std::shared_ptr<IShader> ShaderLibrary::Get(const std::string &name)
{
    return std::shared_ptr<IShader>();
}


