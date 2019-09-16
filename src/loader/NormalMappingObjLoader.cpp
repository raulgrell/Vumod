#include <iostream>
#include <fstream>
#include <sstream>
#include "NormalMappingObjLoader.h"

RawModel NormalMappingObjLoader::LoadOBJ(const std::string &objFileName, Loader &loader)
{
    std::string fileName = objFileName + ".obj";
    std::ifstream inFile(fileName, std::ios::in);

    if (!inFile) {
        std::cerr << "NormalMappingObjLoader: File " << fileName << " could not be opened" << std::endl;
        exit(1);
    }

    std::string line;
    std::vector<Vertex *> vertices;
    std::vector<Vec2> textures;
    std::vector<Vec3> normals;
    std::vector<Vec3> tangents;
    std::vector<unsigned int> indices;

    // This loop collects the vertices, texture coords and normals from the obj file.
    while (!inFile.eof()) {
        getline(inFile, line);
        std::istringstream iss(line);
        std::string starts;

        float x, y, z;

        iss >> starts;

        if (starts == "v") {
            // e.g. v 3.227124 -0.065127 -1.000000
            iss >> x >> y >> z;
            Vec3 vertex(x, y, z);
            Vertex *newVertex = new Vertex(vertices.size(), vertex);
            vertices.push_back(newVertex);
        } else if (starts == "vt") {
            // e.g. vt 0.905299 0.942320
            iss >> x >> y;
            // cout << "got: vt " << setprecision(8) << x << " " << y << endl;
            Vec2 texture(x, y);
            textures.push_back(texture);
        } else if (starts == "vn") {
            // e.g. vn -1.000000 0.000000 0.000000
            iss >> x >> y >> z;
            // cout << "got: vn " << setprecision(8) << x << " " << y << " " << z << endl;
            Vec3 normal(x, y, z);
            normals.push_back(normal);
        } else if (starts == "f") {
            // break when faces start
            //cout << "NormalMappingObjLoader: Read " << vertices.size() << " vertices from " << fileName << endl;
            //cout << "NormalMappingObjLoader: Read " << textures.size() << " texture coords from " << fileName << endl;
            //cout << "NormalMappingObjLoader: Read " << normals.size() << " normals from " << fileName << endl;
            break;
        }
    }

    int faces = 0;

    // read the faces in a second loop
    while (!inFile.eof()) {
        if (line.empty())
            break;

        std::istringstream iss(line);
        std::string starts;
        unsigned int u[9];

        iss >> starts;

        if (starts == "f") {
            // e.g. f 41/1/1 38/2/1 45/3/1
            std::string tmp, f;
            iss >> tmp;
            f += tmp + " ";
            iss >> tmp;
            f += tmp + " ";
            iss >> tmp;
            f += tmp;

            // replace /'s with space.
            size_t x = f.find('/');
            while (x < std::string::npos) {
                f.replace(x, 1, " ");
                x = f.find('/', x + 1);
            }
            std::istringstream iss2(f);
            for (unsigned int &i : u) {
                iss2 >> i;
                // the indices in the obj file start from 1, ours start from 0
                i--;
            }

            Vertex *v0 = processVertex(u[0], u[1], u[2], vertices, indices);
            Vertex *v1 = processVertex(u[3], u[4], u[5], vertices, indices);
            Vertex *v2 = processVertex(u[6], u[7], u[8], vertices, indices);
            calculateTangents(v0, v1, v2, textures);

            faces++;
        }

        getline(inFile, line);
    }

    //cout << "NormalMappingObjLoader: Read " << faces << " faces from " << fileName << endl;

    removeUnusedVertices(vertices);
    std::vector<float> vertexArray;
    std::vector<float> texturesArray;
    std::vector<float> normalArray;
    std::vector<float> tangentArray;
    float furthest = convertDataToArrays(
            vertices, textures, normals,
            vertexArray, texturesArray, normalArray, tangentArray);
    (void) furthest;

    std::vector<unsigned int> indexArray;
    for (unsigned int u : indices) {
        indexArray.push_back(u);
    }

    // free allocated Vertex objects
    for (auto &vertice : vertices) {
        delete vertice;
    }

    return loader.LoadToVAO(vertexArray, texturesArray, normalArray, tangentArray, indexArray);
}

void NormalMappingObjLoader::calculateTangents(
        Vertex *v0, Vertex *v1, Vertex *v2,
        std::vector<Vec2> &textures)
{
    Vec3 deltaPos1 = v1->getPosition() - v0->getPosition();
    Vec3 deltaPos2 = v2->getPosition() - v0->getPosition();
    Vec2 uv0 = textures[v0->getTextureIndex()];
    Vec2 uv1 = textures[v1->getTextureIndex()];
    Vec2 uv2 = textures[v2->getTextureIndex()];
    Vec2 deltaUv1 = uv1 - uv0;
    Vec2 deltaUv2 = uv2 - uv0;

    float r = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv1.y * deltaUv2.x);
    deltaPos1 *= deltaUv2.y;
    deltaPos2 *= deltaUv1.y;
    Vec3 tangent = deltaPos1 - deltaPos2;
    tangent *= r;
    v0->addTangent(tangent);
    v1->addTangent(tangent);
    v2->addTangent(tangent);
}

Vertex *NormalMappingObjLoader::processVertex(
        unsigned int index,
        unsigned int textureIndex,
        unsigned int normalIndex,
        std::vector<Vertex *> &vertices,
        std::vector<unsigned int> &indices)
{
    Vertex *currentVertex = vertices[index];
    if (!currentVertex->isSet()) {
        currentVertex->setTextureIndex(textureIndex);
        currentVertex->setNormalIndex(normalIndex);
        indices.push_back(index);
        return currentVertex;
    } else {
        return dealWithAlreadyProcessedVertex(
                currentVertex, textureIndex, normalIndex, indices, vertices);
    }
}

float NormalMappingObjLoader::convertDataToArrays(
        std::vector<Vertex *> &vertices,
        std::vector<Vec2> &textures,
        std::vector<Vec3> &normals,
        std::vector<float> &verticesArray,
        std::vector<float> &texturesArray,
        std::vector<float> &normalsArray,
        std::vector<float> &tangentsArray)
{
    float furthestPoint = 0;

    for (auto currentVertex : vertices) {
        if (currentVertex->getLength() > furthestPoint) {
            furthestPoint = currentVertex->getLength();
        }
        Vec3 position = currentVertex->getPosition();
        Vec2 textureCoord = textures[currentVertex->getTextureIndex()];
        Vec3 normalVector = normals[currentVertex->getNormalIndex()];
        Vec3 tangent = currentVertex->getAverageTangent();

        verticesArray.push_back(position.x);
        verticesArray.push_back(position.y);
        verticesArray.push_back(position.z);
        texturesArray.push_back(textureCoord.x);
        texturesArray.push_back(1.0 - textureCoord.y);
        normalsArray.push_back(normalVector.x);
        normalsArray.push_back(normalVector.y);
        normalsArray.push_back(normalVector.z);
        tangentsArray.push_back(tangent.x);
        tangentsArray.push_back(tangent.y);
        tangentsArray.push_back(tangent.z);
    }
    return furthestPoint;
}

Vertex *NormalMappingObjLoader::dealWithAlreadyProcessedVertex(
        Vertex *previousVertex,
        int newTextureIndex,
        int newNormalIndex,
        std::vector<unsigned int> &indices,
        std::vector<Vertex *> &vertices)
{
    if (previousVertex->hasSameTextureAndNormal(newTextureIndex, newNormalIndex)) {
        indices.push_back(previousVertex->getIndex());
        return previousVertex;
    } else {
        Vertex *anotherVertex = previousVertex->getDuplicateVertex();
        if (anotherVertex != nullptr) {
            return dealWithAlreadyProcessedVertex(anotherVertex, newTextureIndex, newNormalIndex,
                                                  indices, vertices);
        } else {
            Vertex *duplicateVertex = new Vertex(vertices.size(), previousVertex->getPosition());
            duplicateVertex->setTextureIndex(newTextureIndex);
            duplicateVertex->setNormalIndex(newNormalIndex);
            previousVertex->setDuplicateVertex(duplicateVertex);
            vertices.push_back(duplicateVertex);
            indices.push_back(duplicateVertex->getIndex());
            return duplicateVertex;
        }
    }
}

void NormalMappingObjLoader::removeUnusedVertices(std::vector<Vertex *> &vertices)
{
    for (Vertex *vertex : vertices) {
        vertex->averageTangents();
        if (!vertex->isSet()) {
            vertex->setTextureIndex(0);
            vertex->setNormalIndex(0);
        }
    }
}
