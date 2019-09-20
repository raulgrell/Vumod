#pragma once

#include <models/Model.h>
#include <vector>
#include <string>
#include "TextureData.h"

class Loader
{
public:
    static RawModel LoadToVao(
            const std::vector<float> &positions,
            int dimensions);

    static RawModel LoadToVao(
            const std::vector<float> &positions,
            const std::vector<float> &textureCoords);

    static RawModel LoadToVao(
            const std::vector<float> &positions,
            const std::vector<float> &textureCoords,
            const std::vector<float> &normals,
            const std::vector<unsigned int> &indices);

    static RawModel LoadToVao(
            const std::vector<float> &positions,
            const std::vector<float> &textureCoords,
            const std::vector<float> &normals,
            const std::vector<float> &tangents,
            const std::vector<unsigned int> &indices);

    static unsigned int CreateEmptyVbo(std::vector<float> &data);

    static void AddInstancedAttribute(
            unsigned int vaoId,
            unsigned int vboId,
            unsigned int attribute,
            int dataSize,
            int instancedDataLength,
            int offset);

    static void UpdateVbo(unsigned int vboId, const std::vector<float> &vboData);

    static unsigned int LoadTexture(const std::string &fileName);
    static unsigned int LoadFontAtlasTexture(const std::string &fileName);
    static unsigned int LoadCubeMap(const std::vector<std::string> &textureFiles);

public:
    static TextureData DecodeImage(const std::string &path);
    static unsigned char *LoadPngImage(const std::string &path, int &width, int &height, int &comp);

private:
    static unsigned int CreateVao();
    static void UnbindVao();
    static unsigned int CreateVbo(unsigned int target);
    static void StoreDataInAttributeList(int attributeNumber, int coordinateSize, const std::vector<float> &data);
    static void BindIndexBuffer(const std::vector<unsigned int> &indices);
    static unsigned int LoadTexture(const std::string &fileName, float lodBias, bool useMipMap = true);
};
