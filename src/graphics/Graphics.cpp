#include "Graphics.h"

#include <cassert>
#include <cstdio>
#include <vector>
#include <iostream>

static const char *getGlErrorString(GLenum err)
{
    switch (err) {
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        default: return "Unknown OpenGL error";
    }
}

void Graphics::Antialias(bool enable)
{
    if (enable && !antialiasing) {
        glEnable(GL_MULTISAMPLE);
        antialiasing = true;
    } else if (!enable && antialiasing) {
        glDisable(GL_MULTISAMPLE);
        antialiasing = false;
    }
}

void Graphics::EnableAlphaBlending()
{
    if (!isAlphaBlending) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        isAlphaBlending = true;
        additiveBlending = false;
    }
}

void Graphics::EnableAdditiveBlending()
{
    if (!additiveBlending) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        additiveBlending = true;
        isAlphaBlending = false;
    }
}

void Graphics::DisableBlending()
{
    if (isAlphaBlending || additiveBlending) {
        glDisable(GL_BLEND);
        isAlphaBlending = false;
        additiveBlending = false;
    }
}

void Graphics::EnableDepthTesting(bool enable)
{
    if (enable && !depthTesting) {
        glEnable(GL_DEPTH_TEST);
        depthTesting = true;
    } else if (!enable && depthTesting) {
        glDisable(GL_DEPTH_TEST);
        depthTesting = false;
    }
}

void Graphics::EnableClipping(bool enable)
{
    if (enable && !clipping) {
        glEnable(GL_CLIP_DISTANCE0);
        clipping = true;
    } else if (!enable && clipping) {
        glDisable(GL_CLIP_DISTANCE0);
        clipping = false;
    }
}

void Graphics::CullBackFaces(bool cull)
{
    if (cull && !cullingBackFace) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        cullingBackFace = true;
    } else if (!cull && cullingBackFace) {
        glDisable(GL_CULL_FACE);
        cullingBackFace = false;
    }
}

void Graphics::Wireframe(bool goWireframe)
{
    if (goWireframe && !inWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        inWireframe = true;
    } else if (!goWireframe && inWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        inWireframe = false;
    }
}

int Graphics::SizeOf(unsigned int type)
{
    switch (type) {
        case GL_FLOAT: return 4;
        case GL_UNSIGNED_INT: return 4;
        case GL_INT: return 4;
        case GL_BOOL: return 1;
        case GL_BYTE: return 1;
        default: assert(false);
    }
    return 0;
}

bool Graphics::CheckShader(GLuint handle, const char *desc)
{
    GLint status = 0, logLength = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);
    if ((GLboolean) status == GL_FALSE)
        fprintf(stderr, "ERROR: Shader failed to compile %s!\n", desc);
    if (logLength > 0) {
        std::vector<char> buf;
        buf.resize(logLength + 1);
        glGetShaderInfoLog(handle, logLength, nullptr, &buf[0]);
        fprintf(stderr, "%s\n", &buf[0]);
    }
    return (GLboolean) status == GL_TRUE;
}

bool Graphics::CheckProgram(GLuint handle, const char *desc)
{
    GLint status = 0, logLength = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, &status);
    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength);
    if ((GLboolean) status == GL_FALSE)
        fprintf(stderr, "ERROR: Shader failed to link %s!\n", desc);
    if (logLength > 0) {
        std::vector<char> buf;
        buf.resize(logLength + 1);
        glGetProgramInfoLog(handle, logLength, nullptr, &buf[0]);
        fprintf(stderr, "%s\n", &buf[0]);
    }
    return (GLboolean) status == GL_TRUE;
}

void Graphics::CheckErrors(const char *function, const char *file, int line)
{
    GLenum err(glGetError());
    while (err != GL_NO_ERROR) {
        const char *error = getGlErrorString(err);
        std::cerr << error
                  << " at " << function
                  << " in " << file << ":" << line
                  << std::endl;

        err = glGetError();
    }
}