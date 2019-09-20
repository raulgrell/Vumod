#pragma once


class IRenderer
{
public:
    virtual void Init() = 0;

    virtual void Begin() = 0;
    virtual void End() = 0;

    virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
};

