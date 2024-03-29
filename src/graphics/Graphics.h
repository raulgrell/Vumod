#pragma once

#include <glad/glad.h>

struct Graphics
{
    inline static bool cullingBackFace = false;
    inline static bool inWireframe = false;
    inline static bool isAlphaBlending = false;
    inline static bool additiveBlending = false;
    inline static bool antialiasing = false;
    inline static bool depthTesting = false;
    inline static bool clipping = false;

    static void Antialias(bool enable);
    static void EnableAlphaBlending();
    static void EnableAdditiveBlending();
    static void DisableBlending();
    static void EnableDepthTesting(bool enable);
    static void EnableClipping(bool enable);
    static void CullBackFaces(bool cull);
    static void Wireframe(bool goWireframe);

    static int SizeOf(unsigned int type);
    static void CheckErrors(const char *function, const char *file, int line);
    static bool CheckShader(unsigned int handle, const char *desc);
    static bool CheckProgram(unsigned int handle, const char *desc);
};

#define CHECK_GL() Graphics::CheckErrors(__FUNCTION__, __FILE__, __LINE__)
