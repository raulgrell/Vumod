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

private:
    unsigned int textureId = 0;
    unsigned int normalMapId = 0;
    int numberOfRows = 1;

public:
    explicit ModelTexture(unsigned int id) : textureId(id) {}
    ModelTexture(unsigned int id, int numRows) : textureId(id), numberOfRows(numRows) {}

    unsigned int GetTextureId() const { return textureId; }

    unsigned int GetNormalMapId() const { return normalMapId; }
    void SetNormalMapId(unsigned int value) { normalMapId = value; }

    int GetNumberOfRows() const { return numberOfRows; }
    void SetNumberOfRows(int value) { numberOfRows = value; }

    void SetShineDamper(float value) { shineDamper = value; }
    void SetReflectivity(float value) { reflectivity = value; }
    void SetHasTransparency(bool value) { hasTransparency = value; }
    void SetUseFakeLighting(bool value) { useFakeLighting = value; }

};