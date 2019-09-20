#pragma once

#include "platform/VuWindow.h"
#include "scene/Scene.h"

#include "common.h"
#include "FileBrowser.h"

struct VuGui
{
    explicit VuGui(VuWindow &vw);
    ~VuGui();

    void Begin();
    void End();

    inline static ImGui::FileBrowser fileDialog{0};
};

void drawGui(Scene &vs);