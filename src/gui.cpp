#include "common.h"

struct VuGuiRenderState {
    GLenum active_texture;
    GLint program;
    GLint texture;
    GLint array_buffer;
#ifndef IMGUI_IMPL_OPENGL_ES2
    GLint vertex_array_object;
#endif
#ifdef GL_POLYGON_MODE
    GLint polygon_mode[2];
#endif
    GLint viewport[4];
    GLint scissor_box[4];
    GLenum blend_src_rgb;
    GLenum blend_dst_rgb;
    GLenum blend_src_alpha;
    GLenum blend_dst_alpha;
    GLenum blend_equation_rgb;
    GLenum blend_equation_alpha;
    GLboolean enable_blend = glIsEnabled(GL_BLEND);
    GLboolean enable_cull_face = glIsEnabled(GL_CULL_FACE);
    GLboolean enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    GLboolean enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
    bool clip_origin_lower_left = true;
#if defined(GL_CLIP_ORIGIN) && !defined(__APPLE__)
    GLenum clip_origin = 0;
#endif
};

#include "gui/console.cpp"
#include "gui/file.cpp"
#include "gui/internal.cpp"
#include "gui/ui.cpp"

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

void initGui(VuWindow &vw, VuGui &vg) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    vg.m_Window = vw.window;
    vg.m_Time = 0.0;

    ImGui_GL_Init(vw.window);
    ImGui_GL_CreateDeviceObjects(vertex_shader_glsl_130, fragment_shader_glsl_130);
}

VuGui::~VuGui() {
    ImGui_GL_DestroyDeviceObjects();
    ImGui_GLFW_Shutdown();
    ImGui::DestroyContext();
}

void VuGui::Begin() {
    ImGuiIO &io = ImGui::GetIO();
    IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built.");

    // Setup display size every frame to account for window resizing
    int w, h;
    int display_w, display_h;
    glfwGetWindowSize(m_Window, &w, &h);
    glfwGetFramebufferSize(m_Window, &display_w, &display_h);
    io.DisplaySize = ImVec2((float) w, (float) h);
    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2((float) display_w / w, (float) display_h / h);

    // Setup time step
    double current_time = glfwGetTime();
    io.DeltaTime = m_Time > 0.0 ? (float) (current_time - m_Time) : (1.0f / 60.0f);
    m_Time = current_time;

    ImGui_GLFW_UpdateMousePosAndButtons(m_Window);
    ImGui_GLFW_UpdateMouseCursor(m_Window);
    ImGui_GLFW_UpdateGamepads();

    ImGui::NewFrame();
}

void VuGui::End() {
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
