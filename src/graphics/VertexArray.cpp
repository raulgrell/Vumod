#include "VertexArray.h"

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

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer)
{
    glBindVertexArray(id);
    vertexBuffer->Bind();

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