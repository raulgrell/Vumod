#pragma once

#include <models/RawModel.h>
#include <vector>
#include <string>
#include "TextureData.h"

class Loader
{
public:
    RawModel LoadToVAO(
            std::vector<float> &positions,
            int dimensions);

    RawModel LoadToVAO(
            std::vector<float> &positions,
            std::vector<float> &textureCoords);

    RawModel LoadToVAO(
            std::vector<float> &positions,
            std::vector<float> &textureCoords,
            std::vector<float> &normals,
            std::vector<unsigned int> &indices);

    RawModel LoadToVAO(
            std::vector<float> &positions,
            std::vector<float> &textureCoords,
            std::vector<float> &normals,
            std::vector<float> &tangents,
            std::vector<unsigned int> &indices);

    unsigned int createEmptyVbo(std::vector<float> &data);

    void addInstancedAttribute(
            unsigned int vaoID,
            unsigned int vboID,
            unsigned int attribute,
            int dataSize,
            int instancedDataLength,
            int offset);

    void updateVbo(unsigned int vboID, std::vector<float> &vboData);

    unsigned int LoadTexture(const std::string &fileName);
    unsigned int LoadFontAtlasTexture(const std::string &fileName);
    unsigned int LoadCubeMap(std::vector<std::string> &textureFiles);

public:
    static TextureData DecodeImage(const std::string &path);
    static unsigned char *LoadPNGImage(const std::string &path, int &width, int &height, int &comp);

private:
    unsigned int createVAO();
    void unbindVAO();
    unsigned int createVBO(unsigned int target);
    void storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<float> &data);
    void bindIndexBuffer(std::vector<unsigned int> &indices);
    unsigned int LoadTexture(const std::string &fileName, float lodBias);

    bool useMipMap = true;
};
