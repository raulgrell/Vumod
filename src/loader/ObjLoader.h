#pragma once

#include <math/Math.h>

#include "graphics/Loader.h"
#include "Vertex.h"
#include "ModelData.h"

class ObjLoader
{
public:
    static RawModel LoadOBJ(const std::string &objFileName, Loader &loader);
private:
    static void processVertex(
            unsigned int index,
            unsigned int textureIndex,
            unsigned int normalIndex,
            std::vector<Vertex *> &vertices,
            std::vector<unsigned int> &indices);

    static float convertDataToArrays(
            std::vector<Vertex *> &vertices,
            std::vector<Vec2> &textures,
            std::vector<Vec3> &normals,
            std::vector<float> &verticesArray,
            std::vector<float> &texturesArray,
            std::vector<float> &normalsArray);

    static void dealWithAlreadyProcessedVertex(
            Vertex *previousVertex,
            int newTextureIndex,
            int newNormalIndex,
            std::vector<unsigned int> &indices,
            std::vector<Vertex *> &vertices);

    static void removeUnusedVertices(std::vector<Vertex *> &vertices);
};

