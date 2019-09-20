#pragma once

#include <graphics/Loader.h>
#include "TerrainTexture.h"

class HeightsGenerator
{
public:
    static constexpr float MAX_HEIGHT = 80;
    HeightsGenerator(const std::string &heightMap,
                     int stepSize);
    float GenerateHeight(int x, int z);
    int GetVertexCount() { return vertexCount; }
    int GetStepSize() { return stepSize; }

private:
    float GetInterpolatedNoise(float x, float z);
    float Interpolate(float a, float b, float blend);
    float GetSmoothNoise(int x, int z);
    float GetNoise(int x, int z);
    float GetNoise1(int x, int z);
    float GetHeightFromImage(int x, int z);

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

