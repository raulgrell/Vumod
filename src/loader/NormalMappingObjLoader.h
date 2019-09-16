#pragma once

#include <math/Math.h>
#include "graphics/Loader.h"
#include "loader/Vertex.h"
#include "loader/ModelData.h"

class NormalMappingObjLoader
{
public:
    static RawModel LoadOBJ(const std::string &objFileName, Loader &loader);
private:
    static Vertex *processVertex(
            unsigned int index,
            unsigned int textureIndex,
            unsigned int normalIndex,
            std::vector<Vertex *> &vertices,
            std::vector<unsigned int> &indices);

    static void calculateTangents(
            Vertex *v0, Vertex *v1, Vertex *v2,
            std::vector<Vec2> &textures);

    static float convertDataToArrays(
            std::vector<Vertex *> &vertices,
            std::vector<Vec2> &textures,
            std::vector<Vec3> &normals,
            std::vector<float> &verticesArray,
            std::vector<float> &texturesArray,
            std::vector<float> &normalsArray,
            std::vector<float> &tangentsArray
    );

    static Vertex *dealWithAlreadyProcessedVertex(
            Vertex *previousVertex,
            int newTextureIndex,
            int newNormalIndex,
            std::vector<unsigned int> &indices,
            std::vector<Vertex *> &vertices);

    static void removeUnusedVertices(std::vector<Vertex *> &vertices);
};
