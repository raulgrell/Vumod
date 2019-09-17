#pragma once

#include <models/Model.h>
#include <vector>
#include <string>
#include "TextureData.h"

class Loader
{
public:
    RawModel LoadToVao(
            std::vector<float> &positions,
            int dimensions);

    RawModel LoadToVao(
            std::vector<float> &positions,
            std::vector<float> &textureCoords);

    RawModel LoadToVao(
            std::vector<float> &positions,
            std::vector<float> &textureCoords,
            std::vector<float> &normals,
            std::vector<unsigned int> &indices);

    RawModel LoadToVao(
            std::vector<float> &positions,
            std::vector<float> &textureCoords,
            std::vector<float> &normals,
            std::vector<float> &tangents,
            std::vector<unsigned int> &indices);

    unsigned int CreateEmptyVbo(std::vector<float> &data);

    void AddInstancedAttribute(
            unsigned int vaoId,
            unsigned int vboId,
            unsigned int attribute,
            int dataSize,
            int instancedDataLength,
            int offset);

    void UpdateVbo(unsigned int vboId, std::vector<float> &vboData);

    unsigned int LoadTexture(const std::string &fileName);
    unsigned int LoadFontAtlasTexture(const std::string &fileName);
    unsigned int LoadCubeMap(std::vector<std::string> &textureFiles);

public:
    static TextureData DecodeImage(const std::string &path);
    static unsigned char *LoadPngImage(const std::string &path, int &width, int &height, int &comp);

private:
    unsigned int CreateVao();
    void UnbindVao();
    unsigned int CreateVbo(unsigned int target);
    void StoreDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<float> &data);
    void BindIndexBuffer(std::vector<unsigned int> &indices);
    unsigned int LoadTexture(const std::string &fileName, float lodBias);

    bool useMipMap = true;
};
