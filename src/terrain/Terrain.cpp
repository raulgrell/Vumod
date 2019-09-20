
#include "Terrain.h"

#include <math/Math.h>
#include <util/Utils.h>

#include <cmath>
#include <iostream>
#include <utility>

Terrain::Terrain(int gridX,
                 int gridZ,
                 TerrainTexturePack &texturePack,
                 TerrainTexture &blendMap,
                 const std::string &heightMap)
        : texturePack(texturePack),
          blendMap(blendMap),
          position(gridX * SIZE, 0, gridZ * SIZE)
{
    auto start = Time::Microseconds();
    model = Generate(heightMap);
    auto finish = Time::Microseconds();
    std::cout << "Terrain::Terrain: generateTerrain took " << finish - start << " microseconds" << std::endl;
}

float Terrain::GetHeightOfTerrain(float worldX, float worldZ)
{
    float terrainX = worldX - position.x;
    float terrainZ = worldZ - position.z;
    float gridSquareSize = SIZE / ((float) heightsLength - 1);
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
        answer = math::barycenter(topLeft, topRight, bottomLeft, pos);
    } else {
        answer = math::barycenter(topRight, bottomRight, bottomLeft, pos);
    }
    return answer;
}

std::unique_ptr<IndexedModel> Terrain::Generate(const std::string &heightMap)
{
    int stepSize = 4;

    HeightsGenerator generator(heightMap, stepSize);

    int vertexCount = generator.GetVertexCount();

    // dynamically create 2 dimensional array
    heights = new float *[vertexCount];
    for (int i = 0; i < vertexCount; i++) {
        heights[i] = new float[vertexCount];
    }
    heightsLength = vertexCount;

    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> textureCoords;
    std::vector<unsigned int> indices;

    for (int i = 0; i < vertexCount; i++) {
        for (int j = 0; j < vertexCount; j++) {
            float s = ((float) j) / ((float) (vertexCount - 1));
            float t = ((float) i) / ((float) (vertexCount - 1));
            positions.push_back(s * SIZE);
            float height = GenerateHeight(j, i, generator);
            heights[j][i] = height;
            positions.push_back(height);
            positions.push_back(t * SIZE);
            Vec3 normal = CalculateNormal(j, i, generator);
            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);
            textureCoords.push_back(s);
            textureCoords.push_back(t);
        }
    }

    for (unsigned int gz = 0; gz < vertexCount - 1; gz++) {
        for (unsigned int gx = 0; gx < vertexCount - 1; gx++) {
            unsigned int topLeft = (gz * vertexCount) + gx;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = ((gz + 1) * vertexCount) + gx;
            unsigned int bottomRight = bottomLeft + 1;
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    auto vao = std::make_shared<VertexArray>();
    auto ibo = std::make_shared<IndexBuffer>(&indices[0], indices.size());
    vao->SetIndexBuffer(ibo);

    auto pos = std::make_shared<VertexBuffer>(&positions[0], positions.size() * 4);
    auto tex = std::make_shared<VertexBuffer>(&textureCoords[0], textureCoords.size() * 4);
    auto nor = std::make_shared<VertexBuffer>(&normals[0], normals.size() * 4);

    pos->SetLayout({{0, GL_FLOAT, 3}});
    tex->SetLayout({{1, GL_FLOAT, 2}});
    nor->SetLayout({{2, GL_FLOAT, 3}});

    vao->AddVertexBuffer(pos);
    vao->AddVertexBuffer(tex);
    vao->AddVertexBuffer(nor);

    vao->Unbind();

    return std::make_unique<IndexedModel>(vao, ibo);
}

float Terrain::GenerateHeight(int x, int z, HeightsGenerator &generator)
{
    return generator.GenerateHeight(x, z);
}

Vec3 Terrain::CalculateNormal(int x, int z, HeightsGenerator &generator)
{
    int stepSize = generator.GetStepSize();
    float heightL = GenerateHeight(x - 1, z, generator);
    float heightR = GenerateHeight(x + 1, z, generator);
    float heightD = GenerateHeight(x, z - 1, generator);
    float heightU = GenerateHeight(x, z - 1, generator);
    Vec3 normal = Vec3(heightL - heightR, 2.0f * stepSize, heightD - heightU);
    normal.Normalize();
    return normal;
}

