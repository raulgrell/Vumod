#pragma once

#include <graphics/TextureData.h>

struct RawModel
{
    unsigned int vaoId = 0;
    int vertexCount = 0;

    unsigned int GetVaoId() const { return vaoId; }
    int GetVertexCount() const { return vertexCount; }

    RawModel() = default;
    RawModel(unsigned int vaoId, int vertexCount)
            : vaoId(vaoId), vertexCount(vertexCount) {}
};

struct TexturedModel
{
    RawModel &rawModel;
    ModelTexture &texture;

    TexturedModel(RawModel &model, ModelTexture &texture)
            : rawModel(model), texture(texture) {}

    ModelTexture &GetTexture() { return texture; }
};
