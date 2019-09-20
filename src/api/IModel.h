#pragma once

class IModel {
public:
    virtual unsigned int GetVaoId() const = 0;
    virtual int GetVertexCount() const = 0;
};