#pragma once

#include <graphics/Loader.h>
#include "TerrainTexture.h"

class HeightsGenerator {
public:
	HeightsGenerator(Loader &loader, const std::string& heightMap, int stepSize);
	float generateHeight(int x, int z);
	int getVertexCount() { return vertexCount; }
	int getStepSize() { return stepSize; }

	static constexpr float MAX_HEIGHT = 80;
private:
	float getInterpolatedNoise(float x, float z);
	float interpolate(float a, float b, float blend);
	float getSmoothNoise(int x, int z);
	float getNoise(int x, int z);
	float getNoise1(int x, int z);
	float getHeightFromImage(int x, int z);

	TextureData image;
	int stepSize = 0;
	int vertexCount = 0;
	int imageStepSize = 0;
	int imageVertexCount = 0;

	int seed = 431;
	float amplitude = 70;
	int octaves = 4;
	double roughness = 0.3;
	int xmin = INT_MAX, xmax = INT_MIN, zmin = INT_MAX, zmax = INT_MIN;
	int getNoiseCalls = 0;
	int getNoise1Calls = 0;
	int getSmoothNoiseCalls = 0;
	int getInterpolatedNoiseCalls = 0;

	std::vector<float> noiseTable;
};

