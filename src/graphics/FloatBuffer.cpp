#include "FloatBuffer.h"

#include <glad/glad.h>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "FloatBuffer.h"

FloatBuffer::FloatBuffer(float const *elements, uint32_t count) : count(count)
{
    glCreateBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), elements, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

FloatBuffer::FloatBuffer(int attributeNumber, int coordinateSize, const std::vector<float> &data)
{
    glCreateBuffers(1, &id);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
    glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL();
}

FloatBuffer::~FloatBuffer()
{
    glDeleteBuffers(1, &id);
}

void FloatBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}
void FloatBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}