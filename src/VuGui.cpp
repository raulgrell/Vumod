#include "VuGui.h"

#include <imgui.h>

#include "gui/console.cpp"
#include "gui/file.cpp"
#include "gui/imfilebrowser.cpp"
#include "gui/internal.cpp"

#include <iostream>

static const GLchar *vertex_shader_glsl_130 = R"(
#version 130
uniform mat4 ProjMtx;
in vec2 Position;
in vec2 UV;
in vec4 Color;
out vec2 Frag_UV;
out vec4 Frag_Color;
void main() {
    Frag_UV = UV;
    Frag_Color = Color;
    gl_Position = ProjMtx * vec4(Position.xy,0,1);
})";

static const GLchar *fragment_shader_glsl_130 = R"(
#version 130
uniform sampler2D Texture;
in vec2 Frag_UV;
in vec4 Frag_Color;
out vec4 Out_Color;
void main() {
    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);
})";

void drawMainMenu()
{
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

void drawFileGui()
{
    static std::string filePathName;
    static std::string path;
    static std::string fileName;
    static std::string filter;
    static bool show_file_gui = true;
    static ImGui::FileBrowser fileDialog;


    ImGui::Begin("Files");
    if (show_file_gui) {
        if (FileGui::Instance()->FileDialog("Choose File", "*\0", "./data", "")) {
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
            show_file_gui = false;
        }
    }

    if (ImGui::Button("open file dialog"))
        fileDialog.Open();

    fileDialog.Display();

    if (fileDialog.HasSelected()) {
        std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
        fileDialog.ClearSelected();
    }

    if (ImGui::Button("Open file")) show_file_gui = true;

    if (!filePathName.empty()) ImGui::Text("Chose File Path Name : %s", filePathName.c_str());
    if (!path.empty()) ImGui::Text("Chose Path Name : %s", path.c_str());
    if (!fileName.empty()) ImGui::Text("Chose File Name : %s", fileName.c_str());
    if (!filter.empty()) ImGui::Text("Chose Filter : %s", filter.c_str());
    ImGui::End();
}

void drawGui(VuScene &vs)
{
    drawMainMenu();
    drawFileGui();

    static bool show_ui = true;
    ImGui::Begin("Scene", &show_ui);
    ImGui::DragFloat3("Camera position", vs.vc.position, 0.01f, -16.0f, 16.0f);
    ImGui::DragFloat3("Camera rotation", vs.vc.rotation, 0.01f, -90.0f, 90.0f);
    ImGui::Checkbox("Wireframe", &vs.wireframe);
    ImGui::End();

    static bool show_demo_window = true;
    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    static bool show_console_window = true;
    if (show_console_window)
        ShowConsole(&show_console_window);
}

VuGui::VuGui(VuWindow &vw) : m_Window((GLFWwindow *) vw.window), m_Time(0.0)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_GL_Init((GLFWwindow *) vw.window);
    ImGui_GL_CreateDeviceObjects(vertex_shader_glsl_130, fragment_shader_glsl_130);
}

VuGui::~VuGui()
{
    ImGui_GL_DestroyDeviceObjects();
    ImGui_GLFW_Shutdown();
    ImGui::DestroyContext();
}

void VuGui::Begin()
{
    ImGuiIO &io = ImGui::GetIO();
    IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built.");

    // Setup display size every frame to account for window resizing
    int w, h;
    int display_w, display_h;
    glfwGetWindowSize((GLFWwindow *) m_Window, &w, &h);
    glfwGetFramebufferSize((GLFWwindow *) m_Window, &display_w, &display_h);
    io.DisplaySize = ImVec2((float) w, (float) h);
    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2((float) display_w / w, (float) display_h / h);

    // Setup time step
    double current_time = glfwGetTime();
    io.DeltaTime = m_Time > 0.0 ? (float) (current_time - m_Time) : (1.0f / 60.0f);
    m_Time = current_time;

    ImGui_GLFW_UpdateMousePosAndButtons((GLFWwindow *) m_Window);
    ImGui_GLFW_UpdateMouseCursor((GLFWwindow *) m_Window);
    ImGui_GLFW_UpdateGamepads();

    ImGui::NewFrame();
}

void VuGui::End()
{
    ImGui::Render();

    VuGuiRenderState rs;
    ImGui_GL_BackupRenderState(rs);

    ImDrawData *draw_data = ImGui::GetDrawData();
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int fb_width = (int) (draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int) (draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
        return;

    ImGui_GL_RenderDrawData(rs, draw_data, fb_width, fb_height);
    ImGui_GL_RestoreRenderState(rs);
}