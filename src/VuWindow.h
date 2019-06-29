#pragma once

#include "common.h"

struct VuWindow
{
    void *window = nullptr;

    VuWindow(const char *title, int width, int height);
    ~VuWindow();

    void GetSize(int *width, int *height) const;
    bool Continue() const;

    void Begin();
    void End();
};
