#pragma once

#include <api/IModel.h>
#include <graphics/TextureData.h>
#include <graphics/VertexArray.h>
#include <graphics/IndexBuffer.h>

struct RawModel : IModel
{
    std::shared_ptr<VertexArray> vao;
    unsigned int vertexCount;

    unsigned int GetVaoId() const { return vao->id; }
    int GetVertexCount() const { return vertexCount; }

    RawModel(std::shared_ptr<VertexArray> vao, unsigned int vertexCount)
            : vao(std::move(vao)), vertexCount(vertexCount) {}
};

struct IndexedModel : IModel
{
    std::shared_ptr<VertexArray> vao;
    std::shared_ptr<IndexBuffer> ibo;

    unsigned int GetVaoId() const { return vao->id; }
    int GetVertexCount() const { return ibo->count; }

    IndexedModel(std::shared_ptr<VertexArray> vao, std::shared_ptr<IndexBuffer> ibo)
            : vao(std::move(vao)), ibo(std::move(ibo)) {}
};



struct TexturedModel
{
    IndexedModel &model;
    ModelTexture &texture;

    TexturedModel(IndexedModel &model, ModelTexture &texture)
            : model(model), texture(texture) {}

    ModelTexture &GetTexture() { return texture; }
};
