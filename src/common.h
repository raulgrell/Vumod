#pragma once

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cassert>
#include <cmath>
#include <cstdio>
#include <limits>

#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <memory>
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
    GLFWwindow *window = nullptr;
    int width = 0;
    int height = 0;

    bool IsRunning() const;
    void Begin();
    void End();
};

struct VuWindowCallbacks
{
    GLFWmousebuttonfun m_PrevUserCallbackMouseButton = nullptr;
    GLFWscrollfun m_PrevUserCallbackScroll = nullptr;
    GLFWkeyfun m_PrevUserCallbackKey = nullptr;
    GLFWcharfun m_PrevUserCallbackChar = nullptr;
};

struct VuShader
{
    GLuint vertex_buffer = 0;
    GLuint vertex_shader = 0;
    GLuint fragment_shader = 0;
    GLuint program = 0;
    GLint mvp_location = 0;
    GLint vpos_location = 0;
    GLint vcol_location = 0;

};

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

struct VuGui
{
    GLFWwindow *m_Window = nullptr;
    double m_Time = 0.0;

    ~VuGui();

    void Begin();
    void End();
};

struct VuObject
{
    GLuint vb_id = 0;
    int num_triangles = 0;
    int material_id = 0;
};

struct VuCamera {
    mat4x4 mvp;
};

struct VuScene
{
    VuWindow& vw;
    VuShader& vs;
    VuCamera vc;
    int width;
    int height;
    std::vector<tinyobj::material_t> materials;
    std::vector<VuObject> objects;
    std::unordered_map<std::string, GLuint> textures;
    float bounds_min[3], bounds_max[3];

    VuScene(VuWindow &vw, VuShader &shader);

    void ResizeDisplay();
    void RotateCamera();
    void Draw();

    void LoadFile(const char *string);
    bool LoadObject(const char *string);
};

struct VuFS
{
    std::unordered_map<std::string, std::vector<std::string>> m_MountPoints;
};

struct VuFile
{
    std::string filePath;
    std::string fileName;
    std::string ext;
    bool is_dir;

    explicit VuFile(const tinydir_file &file);
};

struct VuDir {
    std::unique_ptr<tinydir_dir> dir;

    VuDir() : dir(std::make_unique<tinydir_dir>()) {}

    bool TryOpen(const std::string &path);
    std::string BaseName() const;

    void Fold(std::vector<VuFile>& file_list, void (*map_fn)(std::vector<VuFile>&, const tinydir_file &));
};

struct VuTexture
{
    GLuint id;
    GLsizei width, height;
    std::string path;

    static std::unordered_map<std::string, VuTexture> s_Cache;
    static VuTexture Load(const std::string &texturePath);

    void Bind() const;
    void Unbind() const;
};

struct VuScript {
    WrenConfiguration config = {};
    WrenVM *vm = nullptr;

    VuScript();
    ~VuScript();

    void InterpretCommands();
    void InterpretCommand(const char *command);
};

static int CheckErrorsInternal(const char *file, int line);
static bool CheckShader(GLuint handle, const char *desc);
static bool CheckProgram(GLuint handle, const char *desc);
#define CheckErrors() CheckErrorsInternal(__FILE__, __LINE__)

static std::string GetBaseDir(const std::string &filepath);
static size_t ReadFile(const std::string &path, std::vector<unsigned char>& buffer);
static bool FileExists(const std::string &path);

static void Mount(VuFS &vf, const std::string &virtualPath, const std::string &physicalPath);
static void Unmount(VuFS &vf, const std::string &path);
static bool ResolvePhysicalPath(VuFS &vf, const std::string &path, std::string &outPhysicalPath);
static uint8_t *ReadFile(VuFS &vf, const std::string &path);
static size_t ReadFile(VuFS &vf, const std::string &path, std::vector<unsigned char>& buffer);
static std::string ReadTextFile(VuFS &vf, const std::string &path);
static size_t ReadTextFile(VuFS &vf, const std::string &path, std::vector<unsigned char>& buffer);
static bool WriteFile(VuFS &vf, const std::string &path, uint8_t *buffer);
static bool WriteTextFile(VuFS &vf, const std::string &path, const std::string &text);

static std::vector<std::string> splitString(const std::string &strToSplit, char delimeter);
static inline void ltrim(std::string &s);
static inline void rtrim(std::string &s);
static inline void trim(std::string &s);

static void fatal_error(const std::string &errorString);
