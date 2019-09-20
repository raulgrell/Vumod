#pragma once

#include <math/Math.h>
#include "graphics/Loader.h"
#include "loader/Vertex.h"
#include "loader/ModelData.h"

class ObjLoader
{
public:
    static IndexedModel LoadObj(const std::string &objFileName);
private:
    static Vertex *ProcessVertex(
            unsigned int index,
            unsigned int textureIndex,
            unsigned int normalIndex,
            std::vector<Vertex> &vertices,
            std::vector<unsigned int> &indices);

    static void CalculateTangents(
            Vertex *v0, Vertex *v1, Vertex *v2,
            std::vector<Vec2> &textures);

    static float ConvertDataToArrays(
            std::vector<Vertex> &vertices,
            std::vector<Vec2> &textures,
            std::vector<Vec3> &normals,
            std::vector<float> &verticesArray,
            std::vector<float> &texturesArray,
            std::vector<float> &normalsArray,
            std::vector<float> &tangentsArray
    );

    static Vertex *DealWithAlreadyProcessedVertex(
            Vertex *previousVertex,
            int newTextureIndex,
            int newNormalIndex,
            std::vector<unsigned int> &indices,
            std::vector<Vertex> &vertices);

    static void RemoveUnusedVertices(std::vector<Vertex> &vertices);

};
