#include "WaterFrameBuffers.h"

#include <glad/glad.h>
#include <platform/VuWindow.h>
#include <graphics/Graphics.h>

static constexpr int DIVISOR1 = 2;
static constexpr int DIVISOR2 = 1;
static constexpr int REFLECTION_WIDTH = 1280 / DIVISOR1;
static constexpr int REFLECTION_HEIGHT = 720 / DIVISOR1;
static constexpr int REFRACTION_WIDTH = 1280 / DIVISOR2;
static constexpr int REFRACTION_HEIGHT = 720 / DIVISOR2;

WaterFrameBuffers::WaterFrameBuffers()
{
    InitializeReflectionFrameBuffer();
    InitializeRefractionFrameBuffer();
}

WaterFrameBuffers::~WaterFrameBuffers()
{
    glDeleteFramebuffers(1, &reflectionFrameBufferId);
    glDeleteTextures(1, &reflectionTextureId);
    glDeleteRenderbuffers(1, &reflectionDepthBufferId);
    glDeleteFramebuffers(1, &refractionFrameBufferId);
    glDeleteTextures(1, &refractionTextureId);
    glDeleteRenderbuffers(1, &refractionDepthBufferId);
    glDeleteTextures(1, &refractionDepthTextureId);
}

void WaterFrameBuffers::BindReflectionFrameBuffer()
{
    BindFrameBuffer(reflectionFrameBufferId, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void WaterFrameBuffers::BindRefractionFrameBuffer()
{
    BindFrameBuffer(refractionFrameBufferId, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

void WaterFrameBuffers::UnbindCurrentFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, VuWindow::GetWidth(), VuWindow::GetHeight());
}

void WaterFrameBuffers::InitializeReflectionFrameBuffer()
{
    reflectionFrameBufferId = CreateFrameBuffer();
    reflectionTextureId = CreateTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
    reflectionDepthBufferId = CreateDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
    UnbindCurrentFrameBuffer();
}

void WaterFrameBuffers::InitializeRefractionFrameBuffer()
{
    refractionFrameBufferId = CreateFrameBuffer();
    refractionTextureId = CreateTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
    refractionDepthBufferId = CreateDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
    refractionDepthTextureId = CreateDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
    UnbindCurrentFrameBuffer();
}

void WaterFrameBuffers::BindFrameBuffer(unsigned int framebufferId, int width, int height)
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
    glViewport(0, 0, width, height);
    CHECK_GL();
}

unsigned int WaterFrameBuffers::CreateFrameBuffer()
{
    unsigned int framebufferId;
    glGenFramebuffers(1, &framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    CHECK_GL();

    return framebufferId;
}

unsigned int WaterFrameBuffers::CreateTextureAttachment(int width, int height)
{
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureId, 0);
    CHECK_GL();
    return textureId;
}

unsigned int WaterFrameBuffers::CreateDepthTextureAttachment(int width, int height)
{
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureId, 0);
    CHECK_GL();
    return textureId;
}

unsigned int WaterFrameBuffers::CreateDepthBufferAttachment(int width, int height)
{
    unsigned int depthbufferId;
    glGenRenderbuffers(1, &depthbufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, depthbufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthbufferId);
    CHECK_GL();
    return depthbufferId;
}
