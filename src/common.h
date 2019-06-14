#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h> // for glfwGetWin32Window
#endif

#include <stdlib.h>
#include <stdio.h>

#include <linmath.h>
#include <imgui/imconfig.h>
#include <imgui/imgui.h>

struct VuWindow
{
    GLFWwindow *window;
};

struct VuWindowCallbacks
{
    GLFWmousebuttonfun m_PrevUserCallbackMousebutton = NULL;
    GLFWscrollfun m_PrevUserCallbackScroll = NULL;
    GLFWkeyfun m_PrevUserCallbackKey = NULL;
    GLFWcharfun m_PrevUserCallbackChar = NULL;
};

struct VuShader
{
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;
};

struct VuGui
{
    GLFWwindow *m_Window = NULL;
    double m_Time = 0.0;
    GLFWcursor *m_MouseCursors[ImGuiMouseCursor_COUNT] = {0};
    GLuint m_FontTexture = 0;
    GLuint m_ShaderHandle = 0, m_VertHandle = 0, m_FragHandle = 0;
    int m_AttribLocationTex = 0, m_AttribLocationProjMtx = 0;
    int m_AttribLocationVtxPos = 0, m_AttribLocationVtxUV = 0, m_AttribLocationVtxColor = 0;
    unsigned int m_VboHandle = 0, m_ElementsHandle = 0;
};

static bool CheckShader(GLuint handle, const char *desc);
static bool CheckProgram(GLuint handle, const char *desc);