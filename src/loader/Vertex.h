#pragma once

#include <math/Vec.h>
#include <vector>

class Vertex
{
public:
    Vertex(int index, Vec3 position);
    void addTangent(Vec3 tangent);
    void averageTangents();
    Vec3 getAverageTangent();
    int getIndex();
    float getLength();
    bool isSet();
    bool hasSameTextureAndNormal(int textureIndexOther, int normalIndexOther);
    void setTextureIndex(int textureIndex);
    void setNormalIndex(int normalIndex);
    Vec3 &getPosition();
    int getTextureIndex();
    int getNormalIndex();
    Vertex *getDuplicateVertex();
    void setDuplicateVertex(Vertex *duplicateVertex);

private:
    static constexpr int NO_INDEX = -1;
    Vec3 position;
    int textureIndex = NO_INDEX;
    int normalIndex = NO_INDEX;
    Vertex *duplicateVertex = nullptr;
    int index;
    float length;
    std::vector<Vec3> tangents;
    Vec3 averagedTangent;
};

