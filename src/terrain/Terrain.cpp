
#include "Terrain.h"

#include <math/Math.h>
#include <util/Utils.h>

#include <cmath>
#include <iostream>
#include <utility>

Terrain::Terrain(int gridX, int gridZ, Loader &loader,
	TerrainTexturePack &texturePack, TerrainTexture& blendMap, const std::string& heightMap)
	: texturePack(texturePack), blendMap(blendMap), position(gridX * SIZE, 0, gridZ * SIZE)
{
	auto start = Time::microseconds();
	model = Generate(loader, heightMap);
	auto finish = Time::microseconds();
	std::cout << "Terrain::Terrain: generateTerrain took " << finish - start << " microseconds" << std::endl;
}

float Terrain::getHeightOfTerrain(float worldX, float worldZ)
{
	float terrainX = worldX - position.x;
	float terrainZ = worldZ - position.z;
	float gridSquareSize = SIZE / ((float)heightsLength - 1);
	int gridX = (int) std::floor(terrainX / gridSquareSize);
	int gridZ = (int) std::floor(terrainZ / gridSquareSize);
	if (gridX >= heightsLength - 1 || gridZ >= heightsLength - 1 || gridX < 0 || gridZ < 0) {
		return 0;
	}
	float xCoord = std::fmod(terrainX, gridSquareSize) / gridSquareSize;
	float zCoord = std::fmod(terrainZ, gridSquareSize) / gridSquareSize;
	float answer;
	Vec3 topLeft(0.0f, heights[gridX][gridZ], 0.0f);
	Vec3 topRight(1.0f, heights[gridX + 1][gridZ], 0.0f);
	Vec3 bottomLeft(0.0f, heights[gridX][gridZ + 1], 1.0f);
	Vec3 bottomRight(1.0f, heights[gridX + 1][gridZ + 1], 1.0f);
	Vec2 pos(xCoord, zCoord);
	if (xCoord <= (1 - zCoord)) {
		answer = math::Barycenter(topLeft, topRight, bottomLeft, pos);
	} else {
		answer = math::Barycenter(topRight, bottomRight, bottomLeft, pos);
	}
	return answer;
}

RawModel Terrain::Generate(Loader &loader, const std::string &heightMap)
{
	int stepSize = 4;

	HeightsGenerator generator(loader, heightMap, stepSize);

	int vertexCount = generator.getVertexCount();

	// dynamically create 2 dimensional array
	heights = new float* [vertexCount];
	for (int i = 0; i < vertexCount; i++) {
		heights[i] = new float [vertexCount];
	}
	heightsLength = vertexCount;

	std::vector<float> verticesArray;
	std::vector<float> normalsArray;
	std::vector<float> textureArray;
	std::vector<unsigned int> indicesArray;

	for (int i = 0; i < vertexCount; i++) {
		for (int j = 0; j < vertexCount; j++) {
			float s = ((float)j) / ((float)(vertexCount - 1));
			float t = ((float)i) / ((float)(vertexCount - 1));
			verticesArray.push_back(s * SIZE);
			float height = GenerateHeight(j, i, generator);
			heights[j][i] = height;
			verticesArray.push_back(height);
			verticesArray.push_back(t * SIZE);
			Vec3 normal = CalculateNormal(j, i, generator);
			normalsArray.push_back(normal.x);
			normalsArray.push_back(normal.y);
			normalsArray.push_back(normal.z);
			textureArray.push_back(s);
			textureArray.push_back(t);
		}
	}

	for (unsigned int gz = 0; gz < vertexCount - 1; gz++) {
		for (unsigned int gx = 0; gx < vertexCount - 1; gx++) {
			unsigned int topLeft = (gz * vertexCount) + gx;
			unsigned int topRight = topLeft + 1;
			unsigned int bottomLeft = ((gz + 1) * vertexCount) + gx;
			unsigned int bottomRight = bottomLeft + 1;
			indicesArray.push_back(topLeft);
			indicesArray.push_back(bottomLeft);
			indicesArray.push_back(topRight);
			indicesArray.push_back(topRight);
			indicesArray.push_back(bottomLeft);
			indicesArray.push_back(bottomRight);
		}
	}

	return loader.LoadToVAO(verticesArray, textureArray, normalsArray, indicesArray);
}

float Terrain::GenerateHeight(int x, int z, HeightsGenerator &generator)
{
	return generator.generateHeight(x, z);
}

Vec3 Terrain::CalculateNormal(int x, int z, HeightsGenerator &generator)
{
	int stepSize = generator.getStepSize();
	float heightL = GenerateHeight(x - 1, z, generator);
	float heightR = GenerateHeight(x + 1, z, generator);
	float heightD = GenerateHeight(x, z - 1, generator);
	float heightU = GenerateHeight(x, z - 1, generator);
	Vec3 normal = Vec3(heightL - heightR, 2.0f * stepSize, heightD - heightU);
	normal.Normalize();
	return normal;
}

