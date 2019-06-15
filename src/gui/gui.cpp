#include "../common.h"

#define GLFW_HAS_WINDOW_TOPMOST (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200)  // 3.2+ GLFW_FLOATING
#define GLFW_HAS_WINDOW_HOVERED (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300)  // 3.3+ GLFW_HOVERED
#define GLFW_HAS_WINDOW_ALPHA (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300)    // 3.3+ glfwSetWindowOpacity
#define GLFW_HAS_PER_MONITOR_DPI (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ glfwGetMonitorContentScale
#define GLFW_HAS_VULKAN (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200)          // 3.2+ glfwCreateWindowSurface

static const GLchar *vertex_shader_glsl_130 = R"(
#version 130
uniform mat4 ProjMtx;
in vec2 Position;
in vec2 UV;
in vec4 Color;
out vec2 Frag_UV;
out vec4 Frag_Color;
void main()
{
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
void main()
{
    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);
})";

// Chain GLFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
static bool g_MouseJustPressed[5] = {false, false, false, false, false};
static GLFWmousebuttonfun g_PrevUserCallbackMousebutton = NULL;
static GLFWscrollfun g_PrevUserCallbackScroll = NULL;
static GLFWkeyfun g_PrevUserCallbackKey = NULL;
static GLFWcharfun g_PrevUserCallbackChar = NULL;

void ImGui_GL_RenderDrawData(VuGui &vg, ImDrawData *draw_data);
bool ImGui_GL_CreateFontsTexture(VuGui &vg);
void ImGui_GL_DestroyFontsTexture(VuGui &vg);
bool ImGui_GL_CreateDeviceObjects(VuGui &vg);
void ImGui_GL_DestroyDeviceObjects(VuGui &vg);

static void ImGui_GL_SetupRenderState(VuGui &vg, ImDrawData *draw_data, int fb_width, int fb_height, GLuint vertex_array_object);

static void ImGui_GLFW_MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
static void ImGui_GLFW_ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
static void ImGui_GLFW_KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void ImGui_GLFW_CharCallback(GLFWwindow *window, unsigned int c);
void ImGui_GLFW_Shutdown(VuGui &vg);
void ImGui_GLFW_NewFrame(VuGui &vg);

static const char *ImGui_GLFW_GetClipboardText(void *user_data);
static void ImGui_GLFW_SetClipboardText(void *user_data, const char *text);
static void ImGui_GLFW_UpdateGamepads();
static void ImGui_GLFW_UpdateMousePosAndButtons(VuGui &vg);
static void ImGui_GLFW_UpdateMouseCursor(VuGui &vg);

void initGui(VuWindow &vw, VuGui &vg)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    vg.m_Window = vw.window;
    vg.m_Time = 0.0;

    // Setup back-end capabilities flags
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos requests (optional, rarely used)
    io.BackendPlatformName = "GLFW";
    io.BackendRendererName = "OpenGL";

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

    io.SetClipboardTextFn = ImGui_GLFW_SetClipboardText;
    io.GetClipboardTextFn = ImGui_GLFW_GetClipboardText;
    io.ClipboardUserData = vg.m_Window;
#if defined(_WIN32)
    io.ImeWindowHandle = (void *)glfwGetWin32Window(vg.m_Window);
#endif

    io.Fonts->AddFontDefault();

    vg.m_MouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    vg.m_MouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    vg.m_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR); // FIXME: GLFW doesn't have this.
    vg.m_MouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    vg.m_MouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    vg.m_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR); // FIXME: GLFW doesn't have this.
    vg.m_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR); // FIXME: GLFW doesn't have this.
    vg.m_MouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

    // Chain GLFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
    g_PrevUserCallbackMousebutton = glfwSetMouseButtonCallback(vw.window, ImGui_GLFW_MouseButtonCallback);
    g_PrevUserCallbackScroll = glfwSetScrollCallback(vw.window, ImGui_GLFW_ScrollCallback);
    g_PrevUserCallbackKey = glfwSetKeyCallback(vw.window, ImGui_GLFW_KeyCallback);
    g_PrevUserCallbackChar = glfwSetCharCallback(vw.window, ImGui_GLFW_CharCallback);

    GLint current_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &current_texture);

    ImGui_GL_CreateDeviceObjects(vg);
}

void destroyGui(VuGui &vg)
{
    ImGui_GL_DestroyDeviceObjects(vg);
    ImGui_GLFW_Shutdown(vg);
    ImGui::DestroyContext();
}

void beginGui(VuGui &vg) {
    ImGuiIO &io = ImGui::GetIO();
    IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built.");

    // Setup display size every frame to account for window resizing
    int w, h;
    int display_w, display_h;
    glfwGetWindowSize(vg.m_Window, &w, &h);
    glfwGetFramebufferSize(vg.m_Window, &display_w, &display_h);
    io.DisplaySize = ImVec2((float)w, (float)h);
    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);

    // Setup time step
    double current_time = glfwGetTime();
    io.DeltaTime = vg.m_Time > 0.0 ? (float)(current_time - vg.m_Time) : (float)(1.0f / 60.0f);
    vg.m_Time = current_time;

    ImGui_GLFW_UpdateMousePosAndButtons(vg);
    ImGui_GLFW_UpdateMouseCursor(vg);
    ImGui_GLFW_UpdateGamepads();

    ImGui::NewFrame();
}

// Note that this implementation is little overcomplicated because we are saving/setting up/restoring every OpenGL state explicitly
void endGui(VuGui &vg) {
    ImGui::Render();

    ImDrawData *draw_data = ImGui::GetDrawData();
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
        return;

    // Backup GL state
    GLenum last_active_texture;
    glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint *)&last_active_texture);
    glActiveTexture(GL_TEXTURE0);
    GLint last_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    GLint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    GLint last_array_buffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
#ifndef IMGUI_IMPL_OPENGL_ES2
    GLint last_vertex_array_object;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array_object);
#endif
#ifdef GL_POLYGON_MODE
    GLint last_polygon_mode[2];
    glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
#endif
    GLint last_viewport[4];
    glGetIntegerv(GL_VIEWPORT, last_viewport);
    GLint last_scissor_box[4];
    glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
    GLenum last_blend_src_rgb;
    glGetIntegerv(GL_BLEND_SRC_RGB, (GLint *)&last_blend_src_rgb);
    GLenum last_blend_dst_rgb;
    glGetIntegerv(GL_BLEND_DST_RGB, (GLint *)&last_blend_dst_rgb);
    GLenum last_blend_src_alpha;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint *)&last_blend_src_alpha);
    GLenum last_blend_dst_alpha;
    glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint *)&last_blend_dst_alpha);
    GLenum last_blend_equation_rgb;
    glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint *)&last_blend_equation_rgb);
    GLenum last_blend_equation_alpha;
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint *)&last_blend_equation_alpha);
    GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
    GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
    GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
    bool clip_origin_lower_left = true;
#if defined(GL_CLIP_ORIGIN) && !defined(__APPLE__)
    GLenum last_clip_origin = 0;
    glGetIntegerv(GL_CLIP_ORIGIN, (GLint *)&last_clip_origin); // Support for GL 4.5's glClipControl(GL_UPPER_LEFT)
    if (last_clip_origin == GL_UPPER_LEFT)
        clip_origin_lower_left = false;
#endif

    // Setup desired GL state
    // Recreate the VAO every time (this is to easily allow multiple GL contexts to be rendered to. VAO are not shared among GL contexts)
    // The renderer would actually work without any VAO bound, but then our VertexAttrib calls would overwrite the default one currently bound.
    GLuint vertex_array_object = 0;
#ifndef IMGUI_IMPL_OPENGL_ES2
    glGenVertexArrays(1, &vertex_array_object);
#endif
    ImGui_GL_SetupRenderState(vg, draw_data, fb_width, fb_height, vertex_array_object);

    // Will project scissor/clipping rectangles into framebuffer space
    ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
    ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList *cmd_list = draw_data->CmdLists[n];
        size_t idx_buffer_offset = 0;

        // Upload vertex/index buffers
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid *)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid *)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback != NULL)
            {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    ImGui_GL_SetupRenderState(vg, draw_data, fb_width, fb_height, vertex_array_object);
                else
                    pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                // Project scissor/clipping rectangles into framebuffer space
                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
                {
                    // Apply scissor/clipping rectangle
                    if (clip_origin_lower_left)
                        glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));
                    else
                        glScissor((int)clip_rect.x, (int)clip_rect.y, (int)clip_rect.z, (int)clip_rect.w); // Support for GL 4.5 rarely used glClipControl(GL_UPPER_LEFT)

                    // Bind texture, Draw
                    glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                    glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void *)idx_buffer_offset);
                }
            }
            idx_buffer_offset += pcmd->ElemCount * sizeof(ImDrawIdx);
        }
    }

    // Destroy the temporary VAO
#ifndef IMGUI_IMPL_OPENGL_ES2
    glDeleteVertexArrays(1, &vertex_array_object);
#endif

    // Restore modified GL state
    glUseProgram(last_program);
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glActiveTexture(last_active_texture);
#ifndef IMGUI_IMPL_OPENGL_ES2
    glBindVertexArray(last_vertex_array_object);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
    glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
    if (last_enable_blend)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
    if (last_enable_cull_face)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    if (last_enable_depth_test)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    if (last_enable_scissor_test)
        glEnable(GL_SCISSOR_TEST);
    else
        glDisable(GL_SCISSOR_TEST);
#ifdef GL_POLYGON_MODE
    glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_polygon_mode[0]);
#endif
    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
    glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}

static void ImGui_GL_SetupRenderState(VuGui &vg, ImDrawData *draw_data, int fb_width, int fb_height, GLuint vertex_array_object)
{
    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
#ifdef GL_POLYGON_MODE
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

    // Setup viewport, orthographic projection matrix
    // Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayMin is typically (0,0) for single viewport apps.
    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
    const float ortho_projection[4][4] =
        {
            {2.0f / (R - L), 0.0f, 0.0f, 0.0f},
            {0.0f, 2.0f / (T - B), 0.0f, 0.0f},
            {0.0f, 0.0f, -1.0f, 0.0f},
            {(R + L) / (L - R), (T + B) / (B - T), 0.0f, 1.0f},
        };
    glUseProgram(vg.m_ShaderHandle);
    glUniform1i(vg.m_AttribLocationTex, 0);
    glUniformMatrix4fv(vg.m_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);

#ifndef IMGUI_IMPL_OPENGL_ES2
    glBindVertexArray(vertex_array_object);
#endif

    // Bind vertex/index buffers and setup attributes for ImDrawVert
    glBindBuffer(GL_ARRAY_BUFFER, vg.m_VboHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vg.m_ElementsHandle);
    glEnableVertexAttribArray(vg.m_AttribLocationVtxPos);
    glEnableVertexAttribArray(vg.m_AttribLocationVtxUV);
    glEnableVertexAttribArray(vg.m_AttribLocationVtxColor);
    glVertexAttribPointer(vg.m_AttribLocationVtxPos, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid *)IM_OFFSETOF(ImDrawVert, pos));
    glVertexAttribPointer(vg.m_AttribLocationVtxUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid *)IM_OFFSETOF(ImDrawVert, uv));
    glVertexAttribPointer(vg.m_AttribLocationVtxColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid *)IM_OFFSETOF(ImDrawVert, col));
}

bool ImGui_GL_CreateDeviceObjects(VuGui &vg)
{
    // Backup GL state
    GLint last_texture, last_array_buffer;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);

    // Select shaders matching our GLSL versions
    const GLchar *vertex_shader = vertex_shader_glsl_130;
    const GLchar *fragment_shader = fragment_shader_glsl_130;

    // Create shaders
    const GLchar *vertex_shader_with_version[1] = {vertex_shader};
    vg.m_VertHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vg.m_VertHandle, 1, vertex_shader_with_version, NULL);
    glCompileShader(vg.m_VertHandle);
    CheckShader(vg.m_VertHandle, "vertex shader");

    const GLchar *fragment_shader_with_version[1] = {fragment_shader};
    vg.m_FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vg.m_FragHandle, 1, fragment_shader_with_version, NULL);
    glCompileShader(vg.m_FragHandle);
    CheckShader(vg.m_FragHandle, "fragment shader");

    vg.m_ShaderHandle = glCreateProgram();
    glAttachShader(vg.m_ShaderHandle, vg.m_VertHandle);
    glAttachShader(vg.m_ShaderHandle, vg.m_FragHandle);
    glLinkProgram(vg.m_ShaderHandle);
    CheckProgram(vg.m_ShaderHandle, "shader program");

    vg.m_AttribLocationTex = glGetUniformLocation(vg.m_ShaderHandle, "Texture");
    vg.m_AttribLocationProjMtx = glGetUniformLocation(vg.m_ShaderHandle, "ProjMtx");
    vg.m_AttribLocationVtxPos = glGetAttribLocation(vg.m_ShaderHandle, "Position");
    vg.m_AttribLocationVtxUV = glGetAttribLocation(vg.m_ShaderHandle, "UV");
    vg.m_AttribLocationVtxColor = glGetAttribLocation(vg.m_ShaderHandle, "Color");

    // Create buffers
    glGenBuffers(1, &vg.m_VboHandle);
    glGenBuffers(1, &vg.m_ElementsHandle);

    ImGui_GL_CreateFontsTexture(vg);

    // Restore modified GL state
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);

    return true;
}

bool ImGui_GL_CreateFontsTexture(VuGui &vg)
{
    // Build texture atlas
    ImGuiIO &io = ImGui::GetIO();
    unsigned char *pixels;
    int width, height;

    // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely
    // to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than
    // just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    // Upload texture to graphics system
    GLint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGenTextures(1, &vg.m_FontTexture);
    glBindTexture(GL_TEXTURE_2D, vg.m_FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef GL_UNPACK_ROW_LENGTH
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Store our identifier
    io.Fonts->TexID = (ImTextureID)(intptr_t)vg.m_FontTexture;

    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);

    return true;
}

void ImGui_GL_DestroyFontsTexture(VuGui &vg)
{
    if (vg.m_FontTexture)
    {
        ImGuiIO &io = ImGui::GetIO();
        glDeleteTextures(1, &vg.m_FontTexture);
        io.Fonts->TexID = 0;
        vg.m_FontTexture = 0;
    }
}

void ImGui_GL_DestroyDeviceObjects(VuGui &vg)
{
    if (vg.m_VboHandle)
        glDeleteBuffers(1, &vg.m_VboHandle);
    if (vg.m_ElementsHandle)
        glDeleteBuffers(1, &vg.m_ElementsHandle);
    vg.m_VboHandle = vg.m_ElementsHandle = 0;

    if (vg.m_ShaderHandle && vg.m_VertHandle)
        glDetachShader(vg.m_ShaderHandle, vg.m_VertHandle);
    if (vg.m_VertHandle)
        glDeleteShader(vg.m_VertHandle);
    vg.m_VertHandle = 0;

    if (vg.m_ShaderHandle && vg.m_FragHandle)
        glDetachShader(vg.m_ShaderHandle, vg.m_FragHandle);
    if (vg.m_FragHandle)
        glDeleteShader(vg.m_FragHandle);
    vg.m_FragHandle = 0;

    if (vg.m_ShaderHandle)
        glDeleteProgram(vg.m_ShaderHandle);
    vg.m_ShaderHandle = 0;

    ImGui_GL_DestroyFontsTexture(vg);
}

static const char *ImGui_GLFW_GetClipboardText(void *user_data)
{
    return glfwGetClipboardString((GLFWwindow *)user_data);
}

static void ImGui_GLFW_SetClipboardText(void *user_data, const char *text)
{
    glfwSetClipboardString((GLFWwindow *)user_data, text);
}

void ImGui_GLFW_MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (g_PrevUserCallbackMousebutton != NULL)
        g_PrevUserCallbackMousebutton(window, button, action, mods);

    if (action == GLFW_PRESS && button >= 0 && button < IM_ARRAYSIZE(g_MouseJustPressed))
        g_MouseJustPressed[button] = true;
}

void ImGui_GLFW_ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    if (g_PrevUserCallbackScroll != NULL)
        g_PrevUserCallbackScroll(window, xoffset, yoffset);

    ImGuiIO &io = ImGui::GetIO();
    io.MouseWheelH += (float)xoffset;
    io.MouseWheel += (float)yoffset;
}

void ImGui_GLFW_KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (g_PrevUserCallbackKey != NULL)
        g_PrevUserCallbackKey(window, key, scancode, action, mods);

    ImGuiIO &io = ImGui::GetIO();
    if (action == GLFW_PRESS)
        io.KeysDown[key] = true;
    if (action == GLFW_RELEASE)
        io.KeysDown[key] = false;

    // Modifiers are not reliable across systems
    io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
}

void ImGui_GLFW_CharCallback(GLFWwindow *window, unsigned int c)
{
    if (g_PrevUserCallbackChar != NULL)
        g_PrevUserCallbackChar(window, c);

    ImGuiIO &io = ImGui::GetIO();
    if (c > 0 && c < 0x10000)
        io.AddInputCharacter((unsigned short)c);
}

void ImGui_GLFW_Shutdown(VuGui &vg)
{
    for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
    {
        glfwDestroyCursor(vg.m_MouseCursors[cursor_n]);
        vg.m_MouseCursors[cursor_n] = NULL;
    }
}

static void ImGui_GLFW_UpdateMousePosAndButtons(VuGui &vg)
{
    // Update buttons
    ImGuiIO &io = ImGui::GetIO();
    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
    {
        // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
        io.MouseDown[i] = g_MouseJustPressed[i] || glfwGetMouseButton(vg.m_Window, i) != 0;
        g_MouseJustPressed[i] = false;
    }

    // Update mouse position
    const ImVec2 mouse_pos_backup = io.MousePos;
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
    const bool focused = glfwGetWindowAttrib(vg.m_Window, GLFW_FOCUSED) != 0;
    if (focused)
    {
        if (io.WantSetMousePos)
        {
            glfwSetCursorPos(vg.m_Window, (double)mouse_pos_backup.x, (double)mouse_pos_backup.y);
        }
        else
        {
            double mouse_x, mouse_y;
            glfwGetCursorPos(vg.m_Window, &mouse_x, &mouse_y);
            io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
        }
    }
}

static void ImGui_GLFW_UpdateMouseCursor(VuGui &vg)
{
    ImGuiIO &io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(vg.m_Window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        return;

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
    {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        glfwSetInputMode(vg.m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
    else
    {
        // Show OS mouse cursor
        glfwSetCursor(vg.m_Window, vg.m_MouseCursors[imgui_cursor]
                                       ? vg.m_MouseCursors[imgui_cursor]
                                       : vg.m_MouseCursors[ImGuiMouseCursor_Arrow]);
        glfwSetInputMode(vg.m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

static void ImGui_GLFW_UpdateGamepads()
{
    ImGuiIO &io = ImGui::GetIO();
    memset(io.NavInputs, 0, sizeof(io.NavInputs));
    if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0)
        return;

// Update gamepad inputs
#define MAP_BUTTON(NAV_NO, BUTTON_NO)                                      \
    {                                                                      \
        if (buttons_count > BUTTON_NO && buttons[BUTTON_NO] == GLFW_PRESS) \
            io.NavInputs[NAV_NO] = 1.0f;                                   \
    }
#define MAP_ANALOG(NAV_NO, AXIS_NO, V0, V1)                    \
    {                                                          \
        float v = (axes_count > AXIS_NO) ? axes[AXIS_NO] : V0; \
        v = (v - V0) / (V1 - V0);                              \
        if (v > 1.0f)                                          \
            v = 1.0f;                                          \
        if (io.NavInputs[NAV_NO] < v)                          \
            io.NavInputs[NAV_NO] = v;                          \
    }
    int axes_count = 0, buttons_count = 0;
    const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);
    const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttons_count);
    MAP_BUTTON(ImGuiNavInput_Activate, 0);   // Cross / A
    MAP_BUTTON(ImGuiNavInput_Cancel, 1);     // Circle / B
    MAP_BUTTON(ImGuiNavInput_Menu, 2);       // Square / X
    MAP_BUTTON(ImGuiNavInput_Input, 3);      // Triangle / Y
    MAP_BUTTON(ImGuiNavInput_DpadLeft, 13);  // D-Pad Left
    MAP_BUTTON(ImGuiNavInput_DpadRight, 11); // D-Pad Right
    MAP_BUTTON(ImGuiNavInput_DpadUp, 10);    // D-Pad Up
    MAP_BUTTON(ImGuiNavInput_DpadDown, 12);  // D-Pad Down
    MAP_BUTTON(ImGuiNavInput_FocusPrev, 4);  // L1 / LB
    MAP_BUTTON(ImGuiNavInput_FocusNext, 5);  // R1 / RB
    MAP_BUTTON(ImGuiNavInput_TweakSlow, 4);  // L1 / LB
    MAP_BUTTON(ImGuiNavInput_TweakFast, 5);  // R1 / RB
    MAP_ANALOG(ImGuiNavInput_LStickLeft, 0, -0.3f, -0.9f);
    MAP_ANALOG(ImGuiNavInput_LStickRight, 0, +0.3f, +0.9f);
    MAP_ANALOG(ImGuiNavInput_LStickUp, 1, +0.3f, +0.9f);
    MAP_ANALOG(ImGuiNavInput_LStickDown, 1, -0.3f, -0.9f);
#undef MAP_BUTTON
#undef MAP_ANALOG
    if (axes_count > 0 && buttons_count > 0)
        io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
    else
        io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
}