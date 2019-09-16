#pragma once

struct TextureData
{
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char *buffer = nullptr;

    TextureData() = default;
    TextureData(unsigned char *buffer, int width, int height, int channels)
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
    unsigned int textureID = 0;
    unsigned int normalMap = 0;

public:
    explicit ModelTexture(unsigned int id) : textureID(id) {}

    unsigned int getTextureID() const { return textureID; }

    unsigned int getNormalMap() const { return normalMap; }
    void setNormalMap(unsigned int value) { normalMap = value; }

    float getShineDamper() const { return shineDamper; }
    void setShineDamper(float value) { shineDamper = value; }

    float getReflectivity() const { return reflectivity; }
    void setReflectivity(float value) { reflectivity = value; }

    bool isHasTransparency() const { return hasTransparency; }
    void setHasTransparency(bool value) { hasTransparency = value; }

    bool getUseFakeLighting() const { return useFakeLighting; }
    void setUseFakeLighting(bool value) { useFakeLighting = value; }

    int getNumberOfRows() const { return numberOfRows; }
    void setNumberOfRows(int value) { numberOfRows = value; }
};