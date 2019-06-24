#include "../common.h"

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

static void ImGui_GL_BackupRenderState(VuGuiRenderState &rs);

static void ImGui_GL_RestoreRenderState(const VuGuiRenderState &rs);

static void ImGui_GL_SetupRenderState(const ImDrawData *draw_data, int fb_width, int fb_height, GLuint vao);

bool ImGui_GL_CreateFontsTexture();

void ImGui_GL_DestroyFontsTexture();

bool ImGui_GL_CreateDeviceObjects(const char *vertex_shader, const char *fragment_shader);

void ImGui_GL_DestroyDeviceObjects();

static void ImGui_GLFW_MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

static void ImGui_GLFW_ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

static void ImGui_GLFW_KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

static void ImGui_GLFW_CharCallback(GLFWwindow *window, unsigned int c);

static void ImGui_GLFW_UpdateGamepads();

static void ImGui_GLFW_UpdateMousePosAndButtons(GLFWwindow *window);

static void ImGui_GLFW_UpdateMouseCursor(GLFWwindow *window);

static void ImGui_GLFW_SetClipboardText(void *user_data, const char *text);

static const char *ImGui_GLFW_GetClipboardText(void *user_data);

static void ImGui_GLFW_Shutdown();

static GLFWcursor *g_MouseCursors[ImGuiMouseCursor_COUNT] = {nullptr};

// Chain GLFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
static bool g_MouseJustPressed[5] = {false, false, false, false, false};
static GLFWmousebuttonfun g_PrevUserCallbackMousebutton = nullptr;
static GLFWscrollfun g_PrevUserCallbackScroll = nullptr;
static GLFWkeyfun g_PrevUserCallbackKey = nullptr;
static GLFWcharfun g_PrevUserCallbackChar = nullptr;

static GLuint g_FontTexture = 0;
static GLuint g_ShaderHandle = 0;
static GLuint g_VertHandle = 0;
static GLuint g_FragHandle = 0;
static GLuint g_VboHandle = 0;
static GLuint g_ElementsHandle = 0;
static int g_AttribLocationTex = 0;
static int g_AttribLocationProjMtx = 0;
static int g_AttribLocationVtxPos = 0;
static int g_AttribLocationVtxUV = 0;
static int g_AttribLocationVtxColor = 0;

static void ImGui_GL_BackupRenderState(VuGuiRenderState &rs) {
    // Backup GL state
    glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint *) &rs.active_texture);
    glActiveTexture(GL_TEXTURE0);
    glGetIntegerv(GL_CURRENT_PROGRAM, &rs.program);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &rs.texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &rs.array_buffer);
#ifndef IMGUI_IMPL_OPENGL_ES2
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &rs.vertex_array_object);
#endif
#ifdef GL_POLYGON_MODE
    glGetIntegerv(GL_POLYGON_MODE, rs.polygon_mode);
#endif
    glGetIntegerv(GL_VIEWPORT, rs.viewport);
    glGetIntegerv(GL_SCISSOR_BOX, rs.scissor_box);
    glGetIntegerv(GL_BLEND_SRC_RGB, (GLint *) &rs.blend_src_rgb);
    glGetIntegerv(GL_BLEND_DST_RGB, (GLint *) &rs.blend_dst_rgb);
    glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint *) &rs.blend_src_alpha);
    glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint *) &rs.blend_dst_alpha);
    glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint *) &rs.blend_equation_rgb);
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint *) &rs.blend_equation_alpha);
    rs.enable_blend = glIsEnabled(GL_BLEND);
    rs.enable_cull_face = glIsEnabled(GL_CULL_FACE);
    rs.enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    rs.enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
    rs.clip_origin_lower_left = true;
#if defined(GL_CLIP_ORIGIN) && !defined(__APPLE__)
    glGetIntegerv(GL_CLIP_ORIGIN, (GLint *) &rs.clip_origin); // Support for GL 4.5's glClipControl(GL_UPPER_LEFT)
    if (rs.clip_origin == GL_UPPER_LEFT)
        rs.clip_origin_lower_left = false;
#endif
}

static void ImGui_GL_RestoreRenderState(const VuGuiRenderState &rs) {
    glUseProgram(rs.program);
    glBindTexture(GL_TEXTURE_2D, rs.texture);
    glActiveTexture(rs.active_texture);
#ifndef IMGUI_IMPL_OPENGL_ES2
    glBindVertexArray(rs.vertex_array_object);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, rs.array_buffer);
    glBlendEquationSeparate(rs.blend_equation_rgb, rs.blend_equation_alpha);
    glBlendFuncSeparate(rs.blend_src_rgb, rs.blend_dst_rgb, rs.blend_src_alpha, rs.blend_dst_alpha);
    if (rs.enable_blend)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
    if (rs.enable_cull_face)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    if (rs.enable_depth_test)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    if (rs.enable_scissor_test)
        glEnable(GL_SCISSOR_TEST);
    else
        glDisable(GL_SCISSOR_TEST);
#ifdef GL_POLYGON_MODE
    glPolygonMode(GL_FRONT_AND_BACK, (GLenum) rs.polygon_mode[0]);
#endif
    glViewport(rs.viewport[0], rs.viewport[1], (GLsizei) rs.viewport[2], (GLsizei) rs.viewport[3]);
    glScissor(rs.scissor_box[0], rs.scissor_box[1], (GLsizei) rs.scissor_box[2], (GLsizei) rs.scissor_box[3]);
}

static void
ImGui_GL_SetupRenderState(const ImDrawData *draw_data, int fb_width, int fb_height, GLuint vao) {
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
    glViewport(0, 0, (GLsizei) fb_width, (GLsizei) fb_height);
    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
    const float ortho_projection[4][4] = {
            {2.0f / (R - L),    0.0f,              0.0f,  0.0f},
            {0.0f,              2.0f / (T - B),    0.0f,  0.0f},
            {0.0f,              0.0f,              -1.0f, 0.0f},
            {(R + L) / (L - R), (T + B) / (B - T), 0.0f,  1.0f},
    };
    glUseProgram(g_ShaderHandle);
    glUniform1i(g_AttribLocationTex, 0);
    glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);

#ifndef IMGUI_IMPL_OPENGL_ES2
    glBindVertexArray(vao);
#endif

    // Bind vertex/index buffers and setup attributes for ImDrawVert
    glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
    glEnableVertexAttribArray(g_AttribLocationVtxPos);
    glEnableVertexAttribArray(g_AttribLocationVtxUV);
    glEnableVertexAttribArray(g_AttribLocationVtxColor);
    glVertexAttribPointer(g_AttribLocationVtxPos, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert),
                          (GLvoid *) IM_OFFSETOF(ImDrawVert, pos));
    glVertexAttribPointer(g_AttribLocationVtxUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert),
                          (GLvoid *) IM_OFFSETOF(ImDrawVert, uv));
    glVertexAttribPointer(g_AttribLocationVtxColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert),
                          (GLvoid *) IM_OFFSETOF(ImDrawVert, col));
}

void
ImGui_GL_RenderDrawData(const VuGuiRenderState &rs, const ImDrawData *draw_data, int fb_width, int fb_height) {
    // Setup desired GL state
    // Recreate the VAO every time (this is to easily allow multiple GL contexts to be rendered to. VAO are not shared among GL contexts)
    // The renderer would actually work without any VAO bound, but then our VertexAttrib calls would overwrite the default one currently bound.
    GLuint vertex_array_object = 0;
#ifndef IMGUI_IMPL_OPENGL_ES2
    glGenVertexArrays(1, &vertex_array_object);
#endif
    ImGui_GL_SetupRenderState(draw_data, fb_width, fb_height, vertex_array_object);

    // Will project scissor/clipping rectangles into framebuffer space
    ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
    ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList *cmd_list = draw_data->CmdLists[n];
        size_t idx_buffer_offset = 0;

        // Upload vertex/index buffers
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) cmd_list->VtxBuffer.Size * sizeof(ImDrawVert),
                     (const GLvoid *) cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx),
                     (const GLvoid *) cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback != nullptr) {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    ImGui_GL_SetupRenderState(draw_data, fb_width, fb_height, vertex_array_object);
                else
                    pcmd->UserCallback(cmd_list, pcmd);
            } else {
                // Project scissor/clipping rectangles into framebuffer space
                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f) {
                    // Apply scissor/clipping rectangle
                    if (rs.clip_origin_lower_left)
                        glScissor((int) clip_rect.x, (int) (fb_height - clip_rect.w), (int) (clip_rect.z - clip_rect.x),
                                  (int) (clip_rect.w - clip_rect.y));
                    else
                        glScissor((int) clip_rect.x, (int) clip_rect.y, (int) clip_rect.z,
                                  (int) clip_rect.w); // Support for GL 4.5 rarely used glClipControl(GL_UPPER_LEFT)

                    // Bind texture, Draw
                    glBindTexture(GL_TEXTURE_2D, (GLuint) (intptr_t) pcmd->TextureId);
                    glDrawElements(GL_TRIANGLES, (GLsizei) pcmd->ElemCount,
                                   sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
                                   (void *) idx_buffer_offset);
                }
            }
            idx_buffer_offset += pcmd->ElemCount * sizeof(ImDrawIdx);
        }
    }

    // Destroy the temporary VAO
#ifndef IMGUI_IMPL_OPENGL_ES2
    glDeleteVertexArrays(1, &vertex_array_object);
#endif

}

bool ImGui_GL_CreateFontsTexture() {
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
    glGenTextures(1, &g_FontTexture);
    glBindTexture(GL_TEXTURE_2D, g_FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef GL_UNPACK_ROW_LENGTH
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Store our identifier
    io.Fonts->TexID = (ImTextureID) (intptr_t) g_FontTexture;

    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);

    return true;
}

void ImGui_GL_Init(GLFWwindow *window) {
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    ImGui::StyleColorsDark();

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
    io.ClipboardUserData = window;
#if defined(_WIN32)
    io.ImeWindowHandle = (void *) glfwGetWin32Window(window);
#endif

    io.Fonts->AddFontDefault();

    g_MouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    g_MouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    g_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    g_MouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    g_MouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    g_MouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

    // Chain GLFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
    g_PrevUserCallbackMousebutton = glfwSetMouseButtonCallback(window, ImGui_GLFW_MouseButtonCallback);
    g_PrevUserCallbackScroll = glfwSetScrollCallback(window, ImGui_GLFW_ScrollCallback);
    g_PrevUserCallbackKey = glfwSetKeyCallback(window, ImGui_GLFW_KeyCallback);
    g_PrevUserCallbackChar = glfwSetCharCallback(window, ImGui_GLFW_CharCallback);
}

bool ImGui_GL_CreateDeviceObjects(const char *vertex_shader, const char *fragment_shader) {
    // Backup GL state
    GLint last_texture, last_array_buffer;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);

    // Create shaders
    const GLchar *vertex_shader_with_version[1] = {vertex_shader};
    g_VertHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(g_VertHandle, 1, vertex_shader_with_version, nullptr);
    glCompileShader(g_VertHandle);
    CheckShader(g_VertHandle, "vertex shader");

    const GLchar *fragment_shader_with_version[1] = {fragment_shader};
    g_FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(g_FragHandle, 1, fragment_shader_with_version, nullptr);
    glCompileShader(g_FragHandle);
    CheckShader(g_FragHandle, "fragment shader");

    g_ShaderHandle = glCreateProgram();
    glAttachShader(g_ShaderHandle, g_VertHandle);
    glAttachShader(g_ShaderHandle, g_FragHandle);
    glLinkProgram(g_ShaderHandle);
    CheckProgram(g_ShaderHandle, "shader program");

    g_AttribLocationTex = glGetUniformLocation(g_ShaderHandle, "Texture");
    g_AttribLocationProjMtx = glGetUniformLocation(g_ShaderHandle, "ProjMtx");
    g_AttribLocationVtxPos = glGetAttribLocation(g_ShaderHandle, "Position");
    g_AttribLocationVtxUV = glGetAttribLocation(g_ShaderHandle, "UV");
    g_AttribLocationVtxColor = glGetAttribLocation(g_ShaderHandle, "Color");

    // Create buffers
    glGenBuffers(1, &g_VboHandle);
    glGenBuffers(1, &g_ElementsHandle);

    ImGui_GL_CreateFontsTexture();

    // Restore modified GL state
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);

    return true;
}


void ImGui_GL_DestroyFontsTexture() {
    if (g_FontTexture) {
        ImGuiIO &io = ImGui::GetIO();
        glDeleteTextures(1, &g_FontTexture);
        io.Fonts->TexID = nullptr;
        g_FontTexture = 0;
    }
}

void ImGui_GL_DestroyDeviceObjects() {
    if (g_VboHandle)
        glDeleteBuffers(1, &g_VboHandle);
    if (g_ElementsHandle)
        glDeleteBuffers(1, &g_ElementsHandle);
    g_VboHandle = g_ElementsHandle = 0;

    if (g_ShaderHandle && g_VertHandle)
        glDetachShader(g_ShaderHandle, g_VertHandle);
    if (g_VertHandle)
        glDeleteShader(g_VertHandle);
    g_VertHandle = 0;

    if (g_ShaderHandle && g_FragHandle)
        glDetachShader(g_ShaderHandle, g_FragHandle);
    if (g_FragHandle)
        glDeleteShader(g_FragHandle);
    g_FragHandle = 0;

    if (g_ShaderHandle)
        glDeleteProgram(g_ShaderHandle);
    g_ShaderHandle = 0;

    ImGui_GL_DestroyFontsTexture();
}

static const char *ImGui_GLFW_GetClipboardText(void *user_data) {
    return glfwGetClipboardString((GLFWwindow *) user_data);
}

static void ImGui_GLFW_SetClipboardText(void *user_data, const char *text) {
    glfwSetClipboardString((GLFWwindow *) user_data, text);
}

void ImGui_GLFW_MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (g_PrevUserCallbackMousebutton != nullptr)
        g_PrevUserCallbackMousebutton(window, button, action, mods);

    if (action == GLFW_PRESS && button >= 0 && button < IM_ARRAYSIZE(g_MouseJustPressed))
        g_MouseJustPressed[button] = true;
}

void ImGui_GLFW_ScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    if (g_PrevUserCallbackScroll != nullptr)
        g_PrevUserCallbackScroll(window, xoffset, yoffset);

    ImGuiIO &io = ImGui::GetIO();
    io.MouseWheelH += (float) xoffset;
    io.MouseWheel += (float) yoffset;
}

void ImGui_GLFW_KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (g_PrevUserCallbackKey != nullptr)
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

void ImGui_GLFW_CharCallback(GLFWwindow *window, unsigned int c) {
    if (g_PrevUserCallbackChar != nullptr)
        g_PrevUserCallbackChar(window, c);

    ImGuiIO &io = ImGui::GetIO();
    if (c > 0 && c < 0x10000)
        io.AddInputCharacter((unsigned short) c);
}

void ImGui_GLFW_Shutdown() {
    for (auto &cursor : g_MouseCursors) {
        glfwDestroyCursor(cursor);
        cursor = nullptr;
    }
}

static void ImGui_GLFW_UpdateMousePosAndButtons(GLFWwindow *window) {
    // Update buttons
    ImGuiIO &io = ImGui::GetIO();
    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) {
        // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
        io.MouseDown[i] = g_MouseJustPressed[i] || glfwGetMouseButton(window, i) != 0;
        g_MouseJustPressed[i] = false;
    }

    // Update mouse position
    const ImVec2 mouse_pos_backup = io.MousePos;
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
    const bool focused = glfwGetWindowAttrib(window, GLFW_FOCUSED) != 0;
    if (focused) {
        if (io.WantSetMousePos) {
            glfwSetCursorPos(window, (double) mouse_pos_backup.x, (double) mouse_pos_backup.y);
        } else {
            double mouse_x, mouse_y;
            glfwGetCursorPos(window, &mouse_x, &mouse_y);
            io.MousePos = ImVec2((float) mouse_x, (float) mouse_y);
        }
    }
}

static void ImGui_GLFW_UpdateMouseCursor(GLFWwindow *window) {
    ImGuiIO &io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) ||
        glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        return;

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor) {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    } else {
        // Show OS mouse cursor
        glfwSetCursor(window, g_MouseCursors[imgui_cursor]
                              ? g_MouseCursors[imgui_cursor]
                              : g_MouseCursors[ImGuiMouseCursor_Arrow]);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

static void ImGui_GLFW_UpdateGamepads() {
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
