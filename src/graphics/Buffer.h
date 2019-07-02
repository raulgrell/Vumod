#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <glad/glad.h>

struct BufferElement
{
    const char *name;
    unsigned int Size;
    unsigned int Type;
    unsigned int Count;
    unsigned int Offset;
    bool Normalized;

    BufferElement(const char *name, unsigned int type, unsigned int count, bool normalized = false)
            :  name(name), Size(SizeOfType(type)), Type(type), Count(count), Offset(0), Normalized(normalized)
    {
    }

    static unsigned int SizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_INT: return 4;
            case GL_BOOL: return 1;
            case GL_BYTE: return 1;
            default: assert(false);
        }
        return 0;
    }
};

class BufferLayout
{
public:
    BufferLayout() = default;

    BufferLayout(const std::initializer_list<BufferElement> &elements)
            : m_Elements(elements), m_Stride(0)
    {
        uint32_t offset = 0;
        for (auto &element : m_Elements) {
            element.Offset = offset;
            offset += element.Size * element.Count;
            m_Stride += element.Size * element.Count;
        }
    }

    auto GetStride() const -> int { return m_Stride; }
    auto GetElements() const -> const std::vector<BufferElement> &{ return m_Elements; }

    std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
    std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
    std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

private:
    std::vector<BufferElement> m_Elements;
    uint32_t m_Stride {0};
};

class VertexBuffer
{
public:
    VertexBuffer(void const *vertices, uint32_t size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    const auto &GetLayout() const { return layout; }

    void SetLayout(const BufferLayout &l) { layout = l; }

public:
    uint32_t id {0};

private:
    BufferLayout layout;
};

class IndexBuffer
{
public:
    IndexBuffer(uint32_t const *indices, uint32_t count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    auto GetCount() const { return m_Count; }

public:
    uint32_t id {0};

private:
    uint32_t m_Count {0};
};

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

public:
    uint32_t id {0};

private:
    std::vector <std::shared_ptr<VertexBuffer>> m_VertexBuffers;
    std::shared_ptr <IndexBuffer> m_IndexBuffer;
};

