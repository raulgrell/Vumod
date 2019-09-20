#include "Loader.h"
#include "IndexBuffer.h"

#include <iostream>

#include <stb/stb_image.h>
#include <cassert>
#include <scene/SceneShader.h>

IndexedModel Loader::LoadToVao(
        const std::vector<float> &positions,
        const std::vector<float> &textureCoords,
        const std::vector<float> &normals,
        const std::vector<float> &tangents,
        const std::vector<unsigned int> &indices)
{
    auto vao = std::make_shared<VertexArray>();
    auto ibo = std::make_shared<IndexBuffer>(&indices[0], indices.size());
    vao->SetIndexBuffer(ibo);

    auto pos = std::make_shared<VertexBuffer>(&positions[0], positions.size() * 4);
    auto tex = std::make_shared<VertexBuffer>(&textureCoords[0], textureCoords.size() * 4);
    auto nor = std::make_shared<VertexBuffer>(&normals[0], normals.size() * 4);
    auto tan = std::make_shared<VertexBuffer>(&tangents[0], tangents.size() * 4);

    pos->SetLayout({{0, GL_FLOAT, 3}});
    tex->SetLayout({{1, GL_FLOAT, 2}});
    nor->SetLayout({{2, GL_FLOAT, 3}});
    tan->SetLayout({{3, GL_FLOAT, 3}});

    vao->AddVertexBuffer(pos);
    vao->AddVertexBuffer(tex);
    vao->AddVertexBuffer(nor);
    vao->AddVertexBuffer(tan);

    vao->Unbind();

    return IndexedModel(vao, ibo);
}

IndexedModel Loader::LoadToVao(
        const std::vector<float> &positions,
        const std::vector<unsigned int> &indices)
{

}

unsigned int Loader::CreateEmptyVbo(std::vector<float> &data)
{
    unsigned int vboId = 0;
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL();

    return vboId;
}

void Loader::AddInstancedAttribute(
        unsigned int vaoId,
        unsigned int vboId,
        unsigned int attribute,
        int dataSize,
        int instancedDataLength,
        int offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBindVertexArray(vaoId);
    glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE,
                          instancedDataLength * sizeof(float), (const void *) (offset * sizeof(float)));
    glVertexAttribDivisor(attribute, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    CHECK_GL();
}

void Loader::UpdateVbo(unsigned int vboId, const std::vector<float> &vboData)
{
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, vboData.size() * sizeof(float), &vboData[0], GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vboData.size() * sizeof(float), &vboData[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL();
}

// GL_TEXTURE_CUBE_MAP_POSITIVE_X   = Right Face
// GL_TEXTURE_CUBE_MAP_NEGATIVE_X   = Left Face
// GL_TEXTURE_CUBE_MAP_POSITIVE_Y   = Top Face
// GL_TEXTURE_CUBE_MAP_NEGATIVE_Y   = Bottom Face
// GL_TEXTURE_CUBE_MAP_POSITIVE_Z   = Back Face
// GL_TEXTURE_CUBE_MAP_NEGATIVE_Z   = Front Face
unsigned int Loader::LoadCubeMap(const std::vector<std::string> &textureFiles)
{
    unsigned int textureId;
    GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    int i = 0;
    for (const std::string &fileName : textureFiles) {
        TextureData data = DecodeImage(fileName + ".png");
        if (data.channels == 3)
            glTexImage2D(target + i, 0, GL_RGB, data.width, data.height, 0,
                         GL_RGB, GL_UNSIGNED_BYTE, data.buffer);
        else if (data.channels == 4)
            glTexImage2D(target + i, 0, GL_RGBA, data.width, data.height, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, data.buffer);
        else
            assert(0);

        i++;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    CHECK_GL();

    return textureId;
}

unsigned int Loader::LoadTexture(const std::string &fileName)
{
    return LoadTexture(fileName, 0.0);
}

unsigned int Loader::LoadTexture(const std::string &fileName, float lodBias, bool useMipMap)
{
    unsigned int textureId;
    int width, height, channels;
    unsigned char *bytes;

    auto path = fileName + ".png";

    bytes = LoadPngImage(path, width, height, channels);
    if (!bytes) {
        std::cerr << "Loader: LoadTexture failed for " << path << "\n";
        exit(1);
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    if (channels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    else if (channels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    else
        assert(0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    if (useMipMap) {
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, lodBias);

    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    CHECK_GL();

    return textureId;
}

unsigned int Loader::LoadFontAtlasTexture(const std::string &fileName)
{
    return LoadTexture(fileName, 0.0, false);
}

GLubyte *Loader::LoadPngImage(const std::string &path, int &width, int &height, int &comp)
{
    stbi_set_flip_vertically_on_load(true);
    unsigned char *image = stbi_load(path.c_str(), &width, &height, &comp, STBI_default);
    return image;
}

TextureData Loader::DecodeImage(const std::string &path)
{
    int width, height, format;
    unsigned char *buffer = LoadPngImage(path, width, height, format);
    if (!buffer) {
        std::cerr << "Loader: Failed to load texture \"" + path + "\"" << std::endl;
        exit(1);
    }

    return TextureData(width, height, format, buffer);
}

