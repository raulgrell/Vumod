#pragma once

#include <models/Model.h>
#include <vector>
#include <string>
#include "TextureData.h"

class Loader
{
public:
    static IndexedModel LoadToVao(
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

    static TextureData DecodeImage(const std::string &path);
private:
    static void UnbindVao();
    static void StoreDataInAttributeList(int attributeNumber, int coordinateSize, const std::vector<float> &data);
    static void BindIndexBuffer(const std::vector<unsigned int> &indices);
    static unsigned int LoadTexture(const std::string &fileName, float lodBias, bool useMipMap = true);
    static GLubyte *LoadPngImage(const std::string &path, int &width, int &height, int &comp);
};
