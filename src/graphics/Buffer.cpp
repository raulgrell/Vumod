#include "Buffer.h"

#include <glad/glad.h>

VertexBuffer::VertexBuffer(void const *vertices, uint32_t size)
{
    glCreateBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &id);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

IndexBuffer::IndexBuffer(uint32_t const *indices, uint32_t count)
        : m_Count(count)
{
    glCreateBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &id);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &id);
}

void VertexArray::Bind() const
{
    glBindVertexArray(id);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer )
{
    glBindVertexArray(id);

    const auto &layout = vertexBuffer->GetLayout();
    uint32_t index = 0;
    for (const auto &element : layout) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index,
                              element.Count,
                              element.Type,
                              element.Normalized ? GL_TRUE : GL_FALSE,
                              layout.GetStride(),
                              reinterpret_cast<void *>(element.Offset));
        index++;
    }

    m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer)
{
    glBindVertexArray(id);
    indexBuffer->Bind();

    m_IndexBuffer = indexBuffer;
}