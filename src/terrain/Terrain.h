#pragma once

#include <graphics/Loader.h>
#include <math/Vec.h>

#include "TerrainTexture.h"
#include "HeightsGenerator.h"

class Terrain {
public:
	Vec3 position;
	RawModel model;
	TerrainTexturePack& texturePack;
	TerrainTexture& blendMap;

private:
	float **heights{};
	int heightsLength{};

public:
	Terrain(int gridX, int gridZ, Loader &loader,
		TerrainTexturePack &texturePack, TerrainTexture& blendMap, const std::string& heightMap);
	float getHeightOfTerrain(float worldX, float worldZ);
	RawModel Generate(Loader &loader, const std::string &heightMap);

	static constexpr float SIZE = 4*300;
	static constexpr float MAX_HEIGHT = 2*40;
	static constexpr float MAX_PIXEL_COLOR = 256 * 256 * 256;

private:
	static float GenerateHeight(int x, int z, HeightsGenerator &generator);
	static Vec3 CalculateNormal(int x, int z, HeightsGenerator &generator);

};
