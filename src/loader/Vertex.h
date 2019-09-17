#pragma once

#include <math/Vec.h>
#include <vector>

class Vertex
{
    static constexpr int NO_INDEX = -1;

    int index;
    Vec3 position;
    Vec3 averagedTangent;
    float length;
    Vertex *duplicateVertex = nullptr;
    int textureIndex = NO_INDEX;
    int normalIndex = NO_INDEX;
    std::vector<Vec3> tangents;

public:
    Vertex(int index, Vec3 position)
            : index(index),
              position(position),
              length(position.Length()) {}

    int GetIndex() const { return index; }
    float GetLength() const { return length; }
    Vec3 GetPosition() const { return position; }
    Vec3 GetAverageTangent() const { return averagedTangent; }

    Vertex *GetDuplicateVertex() const { return duplicateVertex; }
    void SetDuplicateVertex(Vertex *vertex) { duplicateVertex = vertex; }

    int GetTextureIndex() const { return textureIndex; }
    void SetTextureIndex(int value) { textureIndex = value; }

    int GetNormalIndex() const { return normalIndex; }
    void SetNormalIndex(int value) { textureIndex = value; }


    bool IsSet() const;
    bool HasSameTextureAndNormal(int texture, int normal) const;

    void AddTangent(Vec3 tangent);
    void AverageTangents();
};

