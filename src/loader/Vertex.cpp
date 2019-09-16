#include "Vertex.h"

Vertex::Vertex(int index, Vec3 position)
{
    this->index = index;
    this->position = position;
    this->length = Vec3::Length(position);
}

void Vertex::addTangent(Vec3 tangent)
{
    tangents.push_back(tangent);
}

void Vertex::averageTangents()
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

Vec3 Vertex::getAverageTangent()
{
    return averagedTangent;
}

int Vertex::getIndex()
{
    return index;
}

float Vertex::getLength()
{
    return length;
}

bool Vertex::isSet()
{
    return textureIndex != NO_INDEX && normalIndex != NO_INDEX;
}

bool Vertex::hasSameTextureAndNormal(int textureIndexOther, int normalIndexOther)
{
    return textureIndexOther == textureIndex && normalIndexOther == normalIndex;
}

void Vertex::setTextureIndex(int textureIndex)
{
    this->textureIndex = textureIndex;
}

void Vertex::setNormalIndex(int normalIndex)
{
    this->normalIndex = normalIndex;
}

Vec3 &Vertex::getPosition()
{
    return position;
}

int Vertex::getTextureIndex()
{
    return textureIndex;
}

int Vertex::getNormalIndex()
{
    return normalIndex;
}

Vertex *Vertex::getDuplicateVertex()
{
    return duplicateVertex;
}

void Vertex::setDuplicateVertex(Vertex *duplicateVertex)
{
    this->duplicateVertex = duplicateVertex;
}
 
