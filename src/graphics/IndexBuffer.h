#pragma once

#include <cstdint>

struct IndexBuffer
{
    IndexBuffer(const std::vector<uint32_t> &indices);
    IndexBuffer(const uint32_t *indices, uint32_t count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    uint32_t id {0};
    uint32_t count {0};
};
