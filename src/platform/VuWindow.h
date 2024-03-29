#pragma once

#include "common.h"

class VuWindow
{
public:
    void *window = nullptr;
    inline static int width = 0;
    inline static int height = 0;

public:
    VuWindow(const char *title, int width, int height);
    ~VuWindow();

    void Begin() const;
    void End() const;

    bool Continue() const;

    void GetSize(int *width, int *height);

    static float GetAspectRatio() { return (float) width / height; }
    static int GetWidth() { return width; }
    static int GetHeight() { return height; }
};
