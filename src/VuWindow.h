#pragma once

#include "common.h"

struct VuWindow
{
    void *window = nullptr;
    int width {0};
    int height {0};

    VuWindow(const char *title, int width, int height);
    ~VuWindow();

    void GetSize(int *width, int *height);
    bool Continue() const;

    void Begin();
    void End();
};
