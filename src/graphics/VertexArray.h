#pragma once

#include "Buffer.h"

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;

    void AddVertexBuffer(const std::shared_ptr <VertexBuffer> &vertexBuffer);
    void SetIndexBuffer(const std::shared_ptr <IndexBuffer> &indexBuffer);

    const auto &GetVertexBuffers() const { return m_VertexBuffers; }
    const auto &GetIndexBuffer() const { return m_IndexBuffer; }

private:
    uint32_t m_Id {0};
    std::vector <std::shared_ptr<VertexBuffer>> m_VertexBuffers;
    std::shared_ptr <IndexBuffer> m_IndexBuffer;
};