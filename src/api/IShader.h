#pragma once

#include <string>

struct IShader
{
    virtual void BindAttributes() = 0;
    virtual void GetUniformLocations() = 0;

    virtual void Link() = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual const std::string &GetName() const = 0;
};
