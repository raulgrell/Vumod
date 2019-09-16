#pragma once

#include "platform/VuWindow.h"
#include "scene/Scene.h"

#include "common.h"

struct VuGui
{
    void *m_Window = nullptr;
    double m_Time = 0.0;

    explicit VuGui(VuWindow &vw);
    ~VuGui();

    void Begin();
    void End();
};

void drawGui(Scene &vs);