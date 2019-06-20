#include "../common.h"

static bool show_demo_window = true;
static bool show_console_window = true;
static char const * lFilterPatterns[1] = { "*" };

static std::vector<std::string> commands;
static char command_text[4096] = "System.print(\"Scripting enabled!\")";

void drawMainMenu();
void drawFileGui(bool *openFileGui);

static void drawGui() {
    drawMainMenu();

    static bool openFileGui = true;

    if (openFileGui) {
        drawFileGui(&openFileGui);
    }

    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
        ImGui::InputTextMultiline("##source", command_text, IM_ARRAYSIZE(command_text),
                                  ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16), flags);

        if (ImGui::Button("Run", ImVec2(80, 30))) {
            std::string command{command_text};
            commands.push_back(command);
        }
        ImGui::SameLine();
        if (ImGui::Button("Open File Dialog")) {
            openFileGui = true;
        }
    }

    if (show_console_window)
        ShowConsole(&show_console_window);
}

void drawFileGui(bool *openFileGui) {
    static std::string filePathName;
    static std::string path;
    static std::string fileName;
    static std::string filter;

    if (FileGui::Instance()->FileDialog("Choose File", "*\0", ".", "")) {
        if (FileGui::Instance()->Valid) {
            filePathName = FileGui::Instance()->GetFilepathName();
            path = FileGui::Instance()->GetCurrentPath();
            fileName = FileGui::Instance()->GetCurrentFileName();
            filter = FileGui::Instance()->GetCurrentFilter();
        } else {
            filePathName = "";
            path = "";
            fileName = "";
            filter = "";
        }
        *openFileGui = false;
    }

    if (!filePathName.empty()) ImGui::Text("Chose File Path Name : %s", filePathName.c_str());
    if (!path.empty()) ImGui::Text("Chose Path Name : %s", path.c_str());
    if (!fileName.empty()) ImGui::Text("Chose File Name : %s", fileName.c_str());
    if (!filter.empty()) ImGui::Text("Chose Filter : %s", filter.c_str());
}

void drawMainMenu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "CTRL+O")) {
                auto lTheOpenFileName = tinyfd_openFileDialog("Open file", "", 1, lFilterPatterns, nullptr, 0);
                printf("%s\n", lTheOpenFileName);
            }
            if (ImGui::MenuItem("Save", "CTRL+S")) {
                auto lTheSaveFileName = tinyfd_saveFileDialog("Save file", "", 1, lFilterPatterns, nullptr);
                printf("%s\n", lTheSaveFileName);
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