#include "VertexArray.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
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
    for (const auto &element : layout) {
        glEnableVertexAttribArray(element.location);
        glVertexAttribPointer(element.location,
                              element.count,
                              element.type,
                              element.normalized ? GL_TRUE : GL_FALSE,
                              layout.GetStride(),
                              reinterpret_cast<void *>(element.offset));
    }

    glBindVertexArray(0);

    m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::AddFloatBuffer(int location, int size, const std::shared_ptr<FloatBuffer> &buffer)
{
    glBindVertexArray(id);
    buffer->Bind();
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindVertexArray(0);

    m_FloatBuffers.push_back(buffer);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer)
{
    glBindVertexArray(id);
    indexBuffer->Bind();
    glBindVertexArray(0);

    m_IndexBuffer = indexBuffer;
}