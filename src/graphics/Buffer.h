#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "Graphics.h"

struct BufferElement
{
    const char *name;
    unsigned int Size;
    unsigned int Type;
    unsigned int Count;
    unsigned int Offset;
    bool Normalized;

    BufferElement(const char *name, unsigned int type, unsigned int count, bool normalized = false)
            : name(name),
            Size(Graphics::SizeOf(type)), Type(type), Count(count), Offset(0), Normalized(normalized)
    {
    }
};

class BufferLayout
{
public:
    BufferLayout() = default;
    BufferLayout(const std::initializer_list<BufferElement> &elements);

    auto GetElements() const -> const std::vector<BufferElement> & { return m_Elements; }
    auto GetStride() const -> int { return m_Stride; }

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

    const BufferLayout &GetLayout() const { return layout; }
    void SetLayout(const BufferLayout &l) { layout = l; }

public:
    uint32_t id {0};

private:
    BufferLayout layout;
};

struct IndexBuffer
{
    IndexBuffer(const uint32_t *indices, uint32_t count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    uint32_t id {0};
    uint32_t count {0};
};

