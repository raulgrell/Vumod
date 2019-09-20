#pragma once

class TerrainTexture
{
public:
    explicit TerrainTexture(unsigned int textureId) : textureId(textureId) {}

    unsigned int GetTextureId() const { return textureId; }

private:
    unsigned int textureId;
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

    TerrainTexture &GetBackgroundTexture() { return backgroundTexture; }
    TerrainTexture &GetRTexture() { return rTexture; }
    TerrainTexture &GetGTexture() { return gTexture; }
    TerrainTexture &GetBTexture() { return bTexture; }

private:
    TerrainTexture &backgroundTexture;
    TerrainTexture &rTexture;
    TerrainTexture &gTexture;
    TerrainTexture &bTexture;
};
