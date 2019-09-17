#pragma once

struct TextureData
{
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char *buffer = nullptr;

    TextureData() = default;
    TextureData(int width, int height, int channels, unsigned char *buffer)
            : width(width), height(height), channels(channels), buffer(buffer) {}
};

struct ModelTexture
{
    float shineDamper = 1;
    float reflectivity = 0;
    bool hasTransparency = false;
    bool useFakeLighting = false;
    int numberOfRows = 1;

private:
    unsigned int textureId = 0;
    unsigned int normalMapId = 0;

public:
    explicit ModelTexture(unsigned int id) : textureId(id) {}

    unsigned int getTextureID() const { return textureId; }
    unsigned int getNormalMap() const { return normalMapId; }

};