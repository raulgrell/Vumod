#pragma once

#include "common.h"

struct VuWindow {
    VuWindow(const char *title, int width, int height);

    ~VuWindow();

    GLFWwindow *window = nullptr;
    int width = 0;
    int height = 0;

    bool IsRunning() const;

    void Begin();

    void End();
};
