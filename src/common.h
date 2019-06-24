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

struct VuWindowCallbacks {
    GLFWmousebuttonfun m_PrevUserCallbackMouseButton = nullptr;
    GLFWscrollfun m_PrevUserCallbackScroll = nullptr;
    GLFWkeyfun m_PrevUserCallbackKey = nullptr;
    GLFWcharfun m_PrevUserCallbackChar = nullptr;
};

struct VuVertex {
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 uv;

    VuVertex(const vec3 position, const vec3 normal, const vec3 color, const vec2 uv)
            : position{position[0], position[1], position[2]},
              normal{normal[0], normal[1], normal[2]},
              color{color[0], color[1], color[2]},
              uv{uv[0], uv[1]} {}
};

struct VuObject {
    vec3 position;
    vec3 rotation;
    vec3 scale;

    GLuint vbo_id = 0;
    int material_id = 0;
    int texture_id = 0;

    std::vector<VuVertex> buffer;

    int NumTriangles() const {
        return static_cast<int>(buffer.size() / 3);
    }
};

std::vector<std::string> splitString(const std::string &strToSplit, char delimeter);
void ltrim(std::string &s);
void rtrim(std::string &s);
void trim(std::string &s);

void fatal_error(const std::string &errorString);
