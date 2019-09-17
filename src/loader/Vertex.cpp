#include "Vertex.h"

void Vertex::AddTangent(Vec3 tangent)
{
    tangents.push_back(tangent);
}

void Vertex::AverageTangents()
{
    averagedTangent = Vec3(0, 0, 0);
    if (tangents.empty()) {
        return;
    }
    for (Vec3 &tangent : tangents) {
        averagedTangent += tangent;
    }
    averagedTangent = Vec3::Normal(averagedTangent);
}

bool Vertex::IsSet() const
{
    return textureIndex != NO_INDEX && normalIndex != NO_INDEX;
}

bool Vertex::HasSameTextureAndNormal(int texture, int normal) const
{
    return texture == textureIndex && normal == normalIndex;
}
