#include "HeightsGenerator.h"

#include <iostream>
#include <cmath>
#include <math/Math.h>
#include <util/Utils.h>

HeightsGenerator::HeightsGenerator(Loader &loader, const std::string &heightMap, int stepSize)
        : stepSize(stepSize), vertexCount(256 / stepSize)
{
    if (!heightMap.empty()) {
        std::string fileName = heightMap + ".png";
        image = Loader::DecodeImage(fileName);
        if (!image.buffer) {
            std::cerr << "HeightsGenerator: decodeTextureFile failed for " << fileName << "\n";
            exit(1);
        }
        imageVertexCount = image.height;
        std::cout << "image vertexCount: " << imageVertexCount << std::endl;
        imageStepSize = imageVertexCount / vertexCount;
        if (imageStepSize < 1) {
            imageStepSize = 1;
        }
        std::cout << "image stepSize: " << imageStepSize << std::endl;
    }

    for (int z = -stepSize; z < vertexCount * stepSize + stepSize; z++) {
        for (int x = -stepSize; x < vertexCount * stepSize + stepSize; x++) {
            noiseTable.push_back(getNoise1(x, z));
        }
    }

    std::cout << "size of noise table: " << noiseTable.size() << std::endl;
    std::cout << "should be: " << (vertexCount + 2) * (vertexCount + 2) * stepSize * stepSize << std::endl;
}

float HeightsGenerator::getHeightFromImage(int x, int z)
{
    if (image.buffer == nullptr) {
        return 0;
    }
    if (x < 0 || x >= image.width || z < 0 || z >= image.height) {
        return 0;
    }
    int index = 4 * (x + z * image.width);
    int r = *(image.buffer + index);
    //int g = *(image.buffer + index + 1);
    //int b = *(image.buffer + index + 2);

    // r should be between 0 and 255;
    float height = r;
    height -= 127.5;
    height /= 127.5; // should be between -1 and 1
    height *= MAX_HEIGHT;
    // get rid of 0 height terrain so it's not at the same level as water
    //if (height > -1.0 && height < 1.0)
    //	height -= 2.0;
    return height;
}

float HeightsGenerator::generateHeight(int x, int z)
{
    int imageX = x * imageStepSize;
    int imageZ = z * imageStepSize;

    x *= stepSize;
    z *= stepSize;

    float total = 0.0f;
    float d = 2 << (octaves);
    for (int i = 0; i < octaves; i++) {
        float freq = (float) (2 << i) / d;
        float amp = (float) pow(roughness, i) * amplitude;
        total += getInterpolatedNoise(x * freq, z * freq) * amp;
    }

    float heightFromImage = getHeightFromImage(imageX, imageZ);
    total += heightFromImage;

    return total;
}

float HeightsGenerator::getInterpolatedNoise(float x, float z)
{
    getInterpolatedNoiseCalls++;
    int intX = (int) x;
    int intZ = (int) z;
    float fracX = x - intX;
    float fracZ = z - intZ;

    float v1 = getSmoothNoise(intX, intZ);
    float v2 = getSmoothNoise(intX + 1, intZ);
    float v3 = getSmoothNoise(intX, intZ + 1);
    float v4 = getSmoothNoise(intX + 1, intZ + 1);
    float i1 = interpolate(v1, v2, fracX);
    float i2 = interpolate(v3, v4, fracX);
    return interpolate(i1, i2, fracZ);
}

float HeightsGenerator::interpolate(float a, float b, float blend)
{
    double theta = blend * M_PI;
    float f = (1.0f - cosf(theta)) * 0.5f;
    return a * (1.0f - f) + b * f;
}

float HeightsGenerator::getSmoothNoise(int x, int z)
{
    getSmoothNoiseCalls++;
    float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1)
                     + getNoise(x - 1, z + 1) + getNoise(x + 1, z + 1)) / 16.0f;
    float sides = (getNoise(x - 1, z) + getNoise(x + 1, z)
                   + getNoise(x, z + 1) + getNoise(x, z + 1)) / 8.0f;
    float center = getNoise(x, z) / 4.0f;
    return corners + sides + center;
}

float HeightsGenerator::getNoise(int x, int z)
{
    getNoiseCalls++;
    int valuesPerRow = 2 * stepSize + vertexCount * stepSize;
    int ind = x + stepSize + (z + stepSize) * valuesPerRow;
    float value = noiseTable[ind];

    // return value;
    return 0;
}

float HeightsGenerator::getNoise1(int x, int z)
{
    if (x < xmin) xmin = x;
    if (x > xmax) xmax = x;
    if (z < zmin) zmin = z;
    if (z > zmax) zmax = z;

    getNoise1Calls++;
    Random::Seed(x * 963 + z * 13251 + seed * 31);

    // return Random::Float() * 2.0f - 1.0f;
    return 0;
}
