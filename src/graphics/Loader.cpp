#include "Loader.h"

#include <iostream>

#include <stb/stb_image.h>
#include <cassert>
#include <scene/SceneShader.h>

RawModel Loader::LoadToVao(
        std::vector<float> &positions,
        std::vector<float> &textureCoords,
        std::vector<float> &normals,
        std::vector<float> &tangents,
        std::vector<unsigned int> &indices)
{
    unsigned int vaoID = CreateVao();
    BindIndexBuffer(indices);
    StoreDataInAttributeList(0, 3, positions);
    StoreDataInAttributeList(1, 2, textureCoords);
    StoreDataInAttributeList(2, 3, normals);
    StoreDataInAttributeList(3, 3, tangents);
    UnbindVao();
    return RawModel(vaoID, indices.size());
}

RawModel Loader::LoadToVao(
        std::vector<float> &positions,
        std::vector<float> &textureCoords,
        std::vector<float> &normals,
        std::vector<unsigned int> &indices)
{
    unsigned int vaoID = CreateVao();
    BindIndexBuffer(indices);
    StoreDataInAttributeList(0, 3, positions);
    StoreDataInAttributeList(1, 2, textureCoords);
    StoreDataInAttributeList(2, 3, normals);
    UnbindVao();
    return RawModel(vaoID, indices.size());
}

RawModel Loader::LoadToVao(
        std::vector<float> &positions,
        std::vector<float> &textureCoords)
{
    unsigned int vaoID = CreateVao();
    StoreDataInAttributeList(0, 2, positions);
    StoreDataInAttributeList(1, 2, textureCoords);
    UnbindVao();
    return RawModel(vaoID, 0);
}

RawModel Loader::LoadToVao(
        std::vector<float> &positions,
        int dimensions)
{
    unsigned int vaoID = CreateVao();
    StoreDataInAttributeList(0, dimensions, positions);
    UnbindVao();

    return RawModel(vaoID, positions.size() / dimensions);
}

unsigned int Loader::CreateEmptyVbo(std::vector<float> &data)
{
    unsigned int vboID = 0;
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL();

    return vboID;
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

void Loader::UpdateVbo(unsigned int vboId, std::vector<float> &vboData)
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
unsigned int Loader::LoadCubeMap(std::vector<std::string> &textureFiles)
{
    unsigned int textureID;
    GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
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

    return textureID;
}

unsigned int Loader::LoadTexture(const std::string &fileName)
{
    return LoadTexture(fileName, 0.0);
}

unsigned int Loader::LoadTexture(const std::string &fileName, float lodBias)
{
    unsigned int textureID;
    int width, height, channels;
    unsigned char *bytes;

    auto path = fileName + ".png";

    bytes = LoadPngImage(path, width, height, channels);
    if (!bytes) {
        std::cerr << "Loader: LoadTexture failed for " << path << "\n";
        exit(1);
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

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

    return textureID;
}

unsigned int Loader::LoadFontAtlasTexture(const std::string &fileName)
{
    return LoadTexture(fileName, 0.0);
}

unsigned int Loader::CreateVao()
{
    unsigned int vaoID = 0;
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    return vaoID;
}

void Loader::UnbindVao()
{
    glBindVertexArray(0);
}

unsigned int Loader::CreateVbo(GLenum target)
{
    unsigned int vboID = 0;
    glGenBuffers(1, &vboID);
    glBindBuffer(target, vboID);
    return vboID;
}

void Loader::StoreDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<float> &data)
{
    CreateVbo(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
    glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL();
}

void Loader::BindIndexBuffer(std::vector<unsigned int> &indices)
{
    CreateVbo(GL_ELEMENT_ARRAY_BUFFER);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    CHECK_GL();
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

