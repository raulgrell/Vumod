#include "ShaderLibrary.h"

void ShaderLibrary::Add(const std::shared_ptr<ShaderGL> &shader)
{
    auto name = shader->GetName();
    shaders[name] = shader;
}

void ShaderLibrary::Add(const std::string &name, const std::shared_ptr<ShaderGL> &shader)
{
    shaders[name] = shader;
}

std::shared_ptr<ShaderGL> ShaderLibrary::Load(const std::string &filePath)
{
    auto shader = std::make_shared<ShaderGL>(filePath);
    Add(shader);
    return shader;
}

std::shared_ptr<ShaderGL> ShaderLibrary::Load(const std::string &name, const std::string &filePath)
{
    auto shader = std::make_shared<ShaderGL>(filePath);
    Add(name, shader);
    return shader;
}

bool ShaderLibrary::Exists(const std::string& name) const
{
    return shaders.find(name) != shaders.end();
}

std::shared_ptr<ShaderGL> ShaderLibrary::Get(const std::string &name)
{
    return shaders[name];
}

