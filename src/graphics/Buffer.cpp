#include "Buffer.h"
#include "VertexArray.h"

#include <glad/glad.h>

BufferLayout::BufferLayout(const std::initializer_list<BufferElement> &elements)
        : m_Elements(elements), m_Stride(0)
{
    uint32_t offset = 0;
    for (auto &element : m_Elements) {
        element.Offset = offset;
        offset += element.Size * element.Count;
        m_Stride += element.Size * element.Count;
    }
}

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

IndexBuffer::IndexBuffer(uint32_t const *indices, uint32_t count) : count(count)
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

