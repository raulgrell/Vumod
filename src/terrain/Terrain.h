#pragma once

#include <graphics/Loader.h>
#include <math/Vec.h>

#include "TerrainTexture.h"
#include "HeightsGenerator.h"

class Terrain {
public:
	Vec3 position;
	TerrainTexturePack& texturePack;
	TerrainTexture& blendMap;
	std::unique_ptr<IndexedModel> model = nullptr;

private:
	float **heights{};
	int heightsLength{};

public:
	Terrain(int gridX,
            int gridZ,
            TerrainTexturePack &texturePack,
            TerrainTexture &blendMap,
            const std::string &heightMap);

	float GetHeightOfTerrain(float worldX, float worldZ);
	std::unique_ptr<IndexedModel> Generate(const std::string &heightMap);

	static constexpr float SIZE = 4*300;
	static constexpr float MAX_HEIGHT = 2*40;
	static constexpr float MAX_PIXEL_COLOR = 256 * 256 * 256;

private:
	static float GenerateHeight(int x, int z, HeightsGenerator &generator);
	static Vec3 CalculateNormal(int x, int z, HeightsGenerator &generator);
};
