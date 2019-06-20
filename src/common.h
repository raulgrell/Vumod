#pragma once

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstddef>
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

struct VuShader {
    GLuint program = 0;
    GLuint vao_id = 0;

    GLint uniform_mvp = 0;
    GLint uniform_tint = 0;
    GLint attr_position = 0;
    GLint attr_color = 0;
    GLint attr_normal = 0;
    GLint attr_uv = 0;

    void Bind();
    void Unbind();
};

struct VuVertex {
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 uv;

    VuVertex(const vec3 position, const vec3 normal, const vec3 color, const vec2 uv)
    : position {position[0], position[1], position[2]},
    normal {normal[0], normal[1], normal[2]},
    color {color[0], color[1], color[2]},
    uv {uv[0], uv[1]} {}
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
    GLuint vbo_id = 0;
    int material_id = 0;

    std::vector<VuVertex> buffer;

    int NumTriangles() const {
        return static_cast<int>(buffer.size() / 3);
    }
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
    float bounds_min[3], bounds_max[3];

    VuScene(VuWindow &vw, VuShader &shader);

    void ResizeDisplay();
    void RotateCamera();
    void Draw();

    void LoadFile(const char *path);
    bool LoadObject(const char *path);
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
