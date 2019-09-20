#include <iostream>
#include <fstream>
#include <sstream>
#include "ObjLoader.h"

IndexedModel ObjLoader::LoadObj(const std::string &objFileName)
{
    std::string fileName = objFileName + ".obj";
    std::ifstream inFile(fileName, std::ios::in);

    if (!inFile) {
        std::cerr << "NormalMappingObjLoader: File " << fileName << " could not be opened" << std::endl;
        exit(1);
    }

    std::string line;
    std::vector<Vertex> vertices;
    std::vector<Vec2> textures;
    std::vector<Vec3> normals;
    std::vector<Vec3> tangents;
    std::vector<unsigned int> indices;

    while (!inFile.eof()) {
        getline(inFile, line);
        std::istringstream iss(line);
        std::string starts;

        float x, y, z;

        iss >> starts;

        if (starts == "v") {
            // Vertex: v 3.227124 -0.065127 -1.000000
            iss >> x >> y >> z;
            Vec3 vertex(x, y, z);
            vertices.emplace_back(vertices.size(), vertex);
        } else if (starts == "vt") {
            // TexCoord: vt 0.905299 0.942320
            iss >> x >> y;
            textures.emplace_back(x, y);
        } else if (starts == "vn") {
            // Normal: vn -1.000000 0.000000 0.000000
            iss >> x >> y >> z;
            normals.emplace_back(x, y, z);
        } else if (starts == "f") {
            // break when faces start
            break;
        }
    }

    int faces = 0;
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

            Vertex *v0 = ProcessVertex(u[0], u[1], u[2], vertices, indices);
            Vertex *v1 = ProcessVertex(u[3], u[4], u[5], vertices, indices);
            Vertex *v2 = ProcessVertex(u[6], u[7], u[8], vertices, indices);
            CalculateTangents(v0, v1, v2, textures);

            faces++;
        }

        getline(inFile, line);
    }

    RemoveUnusedVertices(vertices);
    std::vector<float> vertexArray;
    std::vector<float> texturesArray;
    std::vector<float> normalArray;
    std::vector<float> tangentArray;

    float furthest = ConvertDataToArrays(
            vertices, textures, normals,
            vertexArray, texturesArray, normalArray, tangentArray);

    std::vector<unsigned int> indexArray;
    for (unsigned int u : indices) {
        indexArray.push_back(u);
    }

    return Loader::LoadToVao(vertexArray, texturesArray, normalArray, tangentArray, indexArray);
}

void ObjLoader::CalculateTangents(
        Vertex *v0, Vertex *v1, Vertex *v2,
        std::vector<Vec2> &textures)
{
    Vec3 deltaPos1 = v1->GetPosition() - v0->GetPosition();
    Vec3 deltaPos2 = v2->GetPosition() - v0->GetPosition();
    // TODO: fiz
    Vec2 uv0 = 0; //textures[v0->GetTextureIndex()];
    Vec2 uv1 = 0; //textures[v1->GetTextureIndex()];
    Vec2 uv2 = 0; //textures[v2->GetTextureIndex()];
    Vec2 deltaUv1 = uv1 - uv0;
    Vec2 deltaUv2 = uv2 - uv0;

    float r = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv1.y * deltaUv2.x);
    deltaPos1 *= deltaUv2.y;
    deltaPos2 *= deltaUv1.y;
    Vec3 tangent = deltaPos1 - deltaPos2;
    tangent *= r;
    v0->AddTangent(tangent);
    v1->AddTangent(tangent);
    v2->AddTangent(tangent);
}

Vertex *ObjLoader::ProcessVertex(
        unsigned int index,
        unsigned int textureIndex,
        unsigned int normalIndex,
        std::vector<Vertex> &vertices,
        std::vector<unsigned int> &indices)
{
    auto &currentVertex = vertices[index];
    if (!currentVertex.IsSet()) {
        currentVertex.SetTextureIndex(textureIndex);
        currentVertex.SetNormalIndex(normalIndex);
        indices.push_back(index);
        return &currentVertex;
    } else {
        return DealWithAlreadyProcessedVertex(
                &currentVertex, textureIndex, normalIndex, indices, vertices);
    }
}

float ObjLoader::ConvertDataToArrays(
        std::vector<Vertex> &vertices,
        std::vector<Vec2> &textures,
        std::vector<Vec3> &normals,
        std::vector<float> &verticesArray,
        std::vector<float> &texturesArray,
        std::vector<float> &normalsArray,
        std::vector<float> &tangentsArray)
{
    float furthestPoint = 0;

    for (auto &currentVertex : vertices) {
        if (currentVertex.GetLength() > furthestPoint) {
            furthestPoint = currentVertex.GetLength();
        }
        Vec3 position = currentVertex.GetPosition();
        Vec2 textureCoord = textures[currentVertex.GetTextureIndex()];
        Vec3 normalVector = normals[currentVertex.GetNormalIndex()];
        Vec3 tangent = currentVertex.GetAverageTangent();

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

Vertex *ObjLoader::DealWithAlreadyProcessedVertex(
        Vertex *previousVertex,
        int newTextureIndex,
        int newNormalIndex,
        std::vector<unsigned int> &indices,
        std::vector<Vertex> &vertices)
{
    if (previousVertex->HasSameTextureAndNormal(newTextureIndex, newNormalIndex)) {
        indices.push_back(previousVertex->GetIndex());
        return previousVertex;
    } else {
        Vertex *anotherVertex = previousVertex->GetDuplicateVertex();
        if (anotherVertex != nullptr) {
            return DealWithAlreadyProcessedVertex(
                    anotherVertex,
                    newTextureIndex,
                    newNormalIndex,
                    indices,
                    vertices);
        } else {
            auto &duplicateVertex = vertices.emplace_back(vertices.size(), previousVertex->GetPosition());
            duplicateVertex.SetTextureIndex(newTextureIndex);
            duplicateVertex.SetNormalIndex(newNormalIndex);
            previousVertex->SetDuplicateVertex(&duplicateVertex);
            indices.push_back(duplicateVertex.GetIndex());
            return &duplicateVertex;
        }
    }
}

void ObjLoader::RemoveUnusedVertices(std::vector<Vertex> &vertices)
{
    for (auto &vertex : vertices) {
        vertex.AverageTangents();
        if (!vertex.IsSet()) {
            vertex.SetTextureIndex(0);
            vertex.SetNormalIndex(0);
        }
    }
}
