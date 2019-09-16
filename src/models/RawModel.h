#pragma once

struct RawModel
{
    unsigned int vaoID = 0;
    int vertexCount = 0;

    unsigned int getVaoID() const
    {
        return vaoID;
    }
    int getVertexCount() const
    {
        return vertexCount;
    }

    RawModel() = default;
    RawModel(unsigned int vaoID, int vertexCount)
            : vaoID(vaoID), vertexCount(vertexCount) {}
};
