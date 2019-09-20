#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

#include <glad/glad.h>

BufferLayout::BufferLayout(const std::initializer_list<BufferElement> &elements)
        : m_Elements(elements), m_Stride(0)
{
    uint32_t offset = 0;
    for (auto &element : m_Elements) {
        element.offset = offset;
        offset += element.size * element.count;
        m_Stride += element.size * element.count;
    }
}

VertexBuffer::VertexBuffer(const void *vertices, uint32_t size)
{
    glCreateBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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

