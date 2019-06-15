#pragma once

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cassert>
#include <cmath>
#include <cstdio>
#include <limits>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h> // for glfwGetWin32Window
#endif

#include <ext.h>

struct VuWindow
{
    GLFWwindow *window;
    int width;
    int height;
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
    GLuint m_ShaderHandle = 0;
    GLuint m_VertHandle = 0;
    GLuint m_FragHandle = 0;
    int m_AttribLocationTex = 0;
    int m_AttribLocationProjMtx = 0;
    int m_AttribLocationVtxPos = 0;
    int m_AttribLocationVtxUV = 0;
    int m_AttribLocationVtxColor = 0;
    unsigned int m_VboHandle = 0;
    unsigned int m_ElementsHandle = 0;
};

struct VuObject
{
    GLuint vb_id;
    int num_triangles;
    size_t material_id;
};

struct VuScene
{
    std::vector<VuObject> objects;
};

struct VuFS
{
    std::unordered_map<std::string, std::vector<std::string>> m_MountPoints;
};

struct VuTexture
{
    GLuint id;
    GLsizei width, height;
    std::string path;

    static std::unordered_map<std::string, VuTexture> s_Cache;
    static VuTexture Load(std::string texturePath);

    void Bind() const;
    void Unbind();
};

#define CheckErrors() CheckErrorsInternal(__FILE__, __LINE__)
bool CheckShader(GLuint handle, const char *desc);
bool CheckProgram(GLuint handle, const char *desc);

std::string GetBaseDir(const std::string &filepath);
size_t ReadFile(const std::string &path, std::vector<unsigned char>& buffer);
bool FileExists(const std::string &path);

std::vector<std::string> split(std::string strToSplit, char delimeter);

void fatal_error(std::string errorString);
