#pragma once

#include <cstdint>
#include <vector>

class FloatBuffer
{
public:
    FloatBuffer(const float *elements, uint32_t count);
    FloatBuffer(int attributeNumber, int coordinateSize, const std::vector<float> &data);
    ~FloatBuffer();

    void Bind() const;
    void Unbind() const;

    uint32_t id {0};
    uint32_t count {0};
};

