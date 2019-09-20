#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "FloatBuffer.h"

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
    std::vector<std::shared_ptr<FloatBuffer>> m_FloatBuffers;
    std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
    void AddFloatBuffer(int location, int size, const std::shared_ptr<FloatBuffer> &buffer);
};
