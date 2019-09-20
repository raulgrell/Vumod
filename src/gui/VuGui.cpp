#include "VuGui.h"

#include <imgui.h>
#include <imgui/impl/imgui_impl_opengl3.h>
#include <imgui/impl/imgui_impl_glfw.h>

#include "gui/FileBrowser.h"
#include "Console.h"
#include "Debug.h"

#include <iostream>
#include <GLFW/glfw3.h>

static ImGui::FileBrowser fileDialog;

void drawMainMenu()
{
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "CTRL+O")) {
                fileDialog.Open();
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
void drawGui(Scene &vs)
{
    drawMainMenu();

    fileDialog.Display();
    if (fileDialog.HasSelected()) {
        std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
        fileDialog.ClearSelected();
    }

    static bool showDebug = true;
    Debug::Draw("Debug", &showDebug);

    static bool showUi = true;
    ImGui::Begin("Scene", &showUi);
    ImGui::DragFloat3("Camera position", &vs.camera.position[0], 0.1f, -32.0f, 32.0f);
    ImGui::DragFloat3("Camera rotation", &vs.camera.rotation[0], 0.1f, -180.0f, 180.0f);
    ImGui::Checkbox("Wireframe", &vs.wireframe);
    ImGui::End();

    static bool showDemoWindow = true;
    if (showDemoWindow) {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    static bool showConsoleWindow = true;
    if (showConsoleWindow) {
        showConsole(&showConsoleWindow);
    }
}

VuGui::VuGui(VuWindow &vw)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    io.Fonts->AddFontDefault();

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow *) vw.window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    CHECK_GL();
}

VuGui::~VuGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void VuGui::Begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiIO &io = ImGui::GetIO();
    IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built.");
}

void VuGui::End()
{
    ImGui::Render();
    ImDrawData *drawData = ImGui::GetDrawData();

    // scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int fbWidth = (int) (drawData->DisplaySize.x * drawData->FramebufferScale.x);
    int fbHeight = (int) (drawData->DisplaySize.y * drawData->FramebufferScale.y);

    // Avoid rendering when minimized
    if (fbWidth <= 0 || fbHeight <= 0)
        return;

    ImGui_ImplOpenGL3_RenderDrawData(drawData);

    ImGuiIO &io = ImGui::GetIO();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow *backupCurrentContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupCurrentContext);
    }

    CHECK_GL();
}