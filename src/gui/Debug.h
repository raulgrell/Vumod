#pragma once

#include <imgui.h>

class Debug
{
public:
    static void Draw(const char* title, bool *pOpen)
    {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin(title, pOpen)) {
            ImGui::End();
            return;
        }

        // Here we create a context menu only available from the title bar.
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Close Console"))
                *pOpen = false;
            ImGui::EndPopup();
        }

        ImGui::End();
    }
};

