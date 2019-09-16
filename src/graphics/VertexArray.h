#pragma once

#include "Buffer.h"

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;

    void AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer);
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer);

    const auto &GetVertexBuffers() const { return m_VertexBuffers; }
    const auto &GetIndexBuffer() const { return m_IndexBuffer; }

public:
    uint32_t id {0};

private:
    std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
};
