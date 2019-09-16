#include <fstream>
#include <iostream>
#include <sstream>
#include "ObjLoader.h"

RawModel ObjLoader::LoadOBJ(const std::string &objFileName, Loader &loader)
{
    std::string fileName = objFileName + ".obj";
    std::ifstream inFile(fileName, std::ios::in);

    if (!inFile) {
        std::cerr << "OBJFileLoader: File " << fileName << " could not be opened" << std::endl;
        exit(1);
    }

    std::string line;
    std::vector<Vertex *> vertices;
    std::vector<Vec2> textures;
    std::vector<Vec3> normals;
    std::vector<unsigned int> indices;

    // This loop collects the vertices, texture coords and normals from
    // the obj file.
    while (!inFile.eof()) {
        getline(inFile, line);

        std::stringstream iss(line);
        std::string starts;
        float x = 0, y = 0, z = 0;

        // starts contains e.g. v, vt, tv, s, f
        iss >> starts;

        if (starts == "v") {
            // e.g. v 3.227124 -0.065127 -1.000000
            iss >> x >> y >> z;
            // cout << "got: v " << setprecision(8) << x << " " << y << " " << z << std::endl;
            Vec3 vertex(x, y, z);
            Vertex *newVertex = new Vertex(vertices.size(), vertex);
            vertices.push_back(newVertex);
        } else if (starts == "vt") {
            // e.g. vt 0.905299 0.942320
            iss >> x >> y;
            // cout << "got: vt " << setprecision(8) << x << " " << y << std::endl;
            Vec2 texture(x, y);
            textures.push_back(texture);
        } else if (starts == "vn") {
            // e.g. vn -1.000000 0.000000 0.000000
            iss >> x >> y >> z;
            // cout << "got: vn " << setprecision(8) << x << " " << y << " " << z << std::endl;
            Vec3 normal(x, y, z);
            normals.push_back(normal);
        } else if (starts == "f") {
            // break when faces start
            //cout << "OBJFileLoader: Read " << vertices.size() << " vertices from " << fileName << std::endl;
            //cout << "OBJFileLoader: Read " << textures.size() << " texture coords from " << fileName << std::endl;
            //cout << "OBJFileLoader: Read " << normals.size() << " normals from " << fileName << std::endl;
            break;
        }
    }

    std::vector<float> verticesArray;
    std::vector<float> normalsArray;
    std::vector<float> texturesArray;
    std::vector<unsigned int> indicesArray;

    int faces = 0;

    // read the faces in a second loop
    while (!inFile.eof()) {
        if (line.empty())
            break;

        std::stringstream iss(line);
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
            std::stringstream iss2(f);
            for (unsigned int &i : u) {
                iss2 >> i;
                // the indices in the obj file start from 1, ours start from 0
                i--;
            }

            processVertex(u[0], u[1], u[2], vertices, indices);
            processVertex(u[3], u[4], u[5], vertices, indices);
            processVertex(u[6], u[7], u[8], vertices, indices);

            faces++;
        }

        getline(inFile, line);
    }

    removeUnusedVertices(vertices);

    float furthest = convertDataToArrays(vertices, textures, normals,
                                         verticesArray, texturesArray, normalsArray);
    (void) furthest;

    for (unsigned int u : indices) {
        indicesArray.push_back(u);
    }

    // free allocated Vertex objects
    for (auto &vertice : vertices) {
        delete vertice;
    }

    return loader.LoadToVAO(verticesArray, texturesArray, normalsArray, indicesArray);
}

void ObjLoader::processVertex(
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
    } else {
        dealWithAlreadyProcessedVertex(currentVertex, textureIndex, normalIndex, indices, vertices);
    }
}

float ObjLoader::convertDataToArrays(
        std::vector<Vertex *> &vertices,
        std::vector<Vec2> &textures,
        std::vector<Vec3> &normals,
        std::vector<float> &verticesArray,
        std::vector<float> &texturesArray,
        std::vector<float> &normalsArray)
{
    float furthestPoint = 0;

    for (auto currentVertex : vertices) {
        if (currentVertex->getLength() > furthestPoint) {
            furthestPoint = currentVertex->getLength();
        }
        Vec3 position = currentVertex->getPosition();
        Vec2 textureCoord = textures[currentVertex->getTextureIndex()];
        Vec3 normalVector = normals[currentVertex->getNormalIndex()];

        verticesArray.push_back(position.x);
        verticesArray.push_back(position.y);
        verticesArray.push_back(position.z);
        texturesArray.push_back(textureCoord.x);
        texturesArray.push_back(1.0 - textureCoord.y);
        normalsArray.push_back(normalVector.x);
        normalsArray.push_back(normalVector.y);
        normalsArray.push_back(normalVector.z);
    }
    return furthestPoint;
}

void ObjLoader::dealWithAlreadyProcessedVertex(
        Vertex *previousVertex,
        int newTextureIndex,
        int newNormalIndex,
        std::vector<unsigned int> &indices,
        std::vector<Vertex *> &vertices)
{
    if (previousVertex->hasSameTextureAndNormal(newTextureIndex, newNormalIndex)) {
        indices.push_back(previousVertex->getIndex());
    } else {
        Vertex *anotherVertex = previousVertex->getDuplicateVertex();
        if (anotherVertex != nullptr) {
            dealWithAlreadyProcessedVertex(anotherVertex, newTextureIndex, newNormalIndex,
                                           indices, vertices);
        } else {
            Vertex *duplicateVertex = new Vertex(vertices.size(), previousVertex->getPosition());
            duplicateVertex->setTextureIndex(newTextureIndex);
            duplicateVertex->setNormalIndex(newNormalIndex);
            previousVertex->setDuplicateVertex(duplicateVertex);
            vertices.push_back(duplicateVertex);
            indices.push_back(duplicateVertex->getIndex());
        }
    }
}

void ObjLoader::removeUnusedVertices(std::vector<Vertex *> &vertices)
{
    std::vector<Vertex *>::iterator it;

    for (it = vertices.begin(); it != vertices.end(); it++) {
        Vertex *vertex = *it;
        if (!vertex->isSet()) {
            vertex->setTextureIndex(0);
            vertex->setNormalIndex(0);
        }
    }
}
