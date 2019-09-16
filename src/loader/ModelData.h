#pragma once

#include <vector>

struct ModelData
{
    std::vector<float> vertices;
    std::vector<float> textureCoords;
    std::vector<float> normals;
    std::vector<unsigned int> indices;
    float furthestPoint;

    ModelData(std::vector<float> &vertices,
              std::vector<float> &texCoords,
              std::vector<float> &normals,
              std::vector<unsigned int> &indices,
              float furthestPoint)
            : furthestPoint(furthestPoint)
    {
        for (float vertex : vertices) {
            this->vertices.push_back(vertex);
        }
        for (float texCoord : texCoords) {
            this->textureCoords.push_back(texCoord);
        }
        for (float normal : normals) {
            this->normals.push_back(normal);
        }
        for (unsigned int index : indices) {
            this->indices.push_back(index);
        }
    }
};