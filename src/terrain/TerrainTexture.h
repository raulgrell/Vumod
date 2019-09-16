#pragma once

class TerrainTexture
{
public:
    explicit
    TerrainTexture(unsigned int textureID) : textureID(textureID) {}

    unsigned int getTextureId() { return textureID; }
private:
    unsigned int textureID;
};

class TerrainTexturePack
{
public:
    TerrainTexturePack(
            TerrainTexture &backgroundTexture,
            TerrainTexture &rTexture,
            TerrainTexture &gTexture,
            TerrainTexture &bTexture) :
            backgroundTexture(backgroundTexture),
            rTexture(rTexture),
            gTexture(gTexture),
            bTexture(bTexture)
    {
    }

    TerrainTexture &getBackgroundTexture() { return backgroundTexture; }
    TerrainTexture &getrTexture() { return rTexture; }
    TerrainTexture &getgTexture() { return gTexture; }
    TerrainTexture &getbTexture() { return bTexture; }

private:
    TerrainTexture &backgroundTexture;
    TerrainTexture &rTexture;
    TerrainTexture &gTexture;
    TerrainTexture &bTexture;
};
