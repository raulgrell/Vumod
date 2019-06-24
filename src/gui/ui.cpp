#include "../common.h"

static char const * lFilterPatterns[1] = { "*" };

static std::vector<std::string> commands;
static char command_text[4096] = "System.print(\"Scripting enabled!\")";

void drawMainMenu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "CTRL+O")) {
                printf("%s\n", "Open");
            }
            if (ImGui::MenuItem("Save", "CTRL+S")) {
                printf("%s\n", "Save");
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

static void drawGui(VuScene &vs) {
    drawMainMenu();

    static bool show_ui = true;
    ImGui::Begin("Scene", &show_ui);
    ImGui::DragFloat3("Camera position", vs.vc.position, 0.01f, -16.0f, 16.0f);
    ImGui::DragFloat3("Camera rotation", vs.vc.rotation, 0.01f, -90.0f, 90.0f);
    ImGui::End();

    static bool show_demo_window = true;
    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    static bool show_console_window = true;
    if (show_console_window)
        ShowConsole(&show_console_window);
}

