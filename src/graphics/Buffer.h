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
    unsigned int Size;
    unsigned int Type;
    unsigned int Count;
    unsigned int Offset;
    bool Normalized;

    BufferElement(unsigned int type, unsigned int count, bool normalized = false)
            :  Size(SizeOfType(type)), Type(type), Count(count), Offset(0), Normalized(normalized)
    {
    }

    static unsigned int SizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_INT: return 4;
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
            offset += element.Size;
            m_Stride += element.Size;
        }
    }

    auto GetStride() const { return m_Stride; }
    const auto &GetElements() const { return m_Elements; }

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

    const auto &GetLayout() const { return m_Layout; }

    void SetLayout(const BufferLayout &layout) { m_Layout = layout; }

private:
    uint32_t m_Id {0};
    BufferLayout m_Layout;
};

class IndexBuffer
{
public:
    IndexBuffer(uint32_t const *indices, uint32_t count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    auto GetCount() const { return m_Count; }

private:
    uint32_t m_RendererID {0};
    uint32_t m_Count {0};
};


