#pragma once


class IRenderer
{
public:
    virtual void Begin() = 0;
    virtual void End() = 0;
};

