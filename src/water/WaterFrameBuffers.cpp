#include "WaterFrameBuffers.h"

#include <glad/glad.h>
#include <platform/VuWindow.h>

static constexpr int DIVISOR1 = 2;
static constexpr int DIVISOR2 = 1;
static constexpr int REFLECTION_WIDTH = 1280 / DIVISOR1;
static constexpr int REFLECTION_HEIGHT = 720 / DIVISOR1;
static constexpr int REFRACTION_WIDTH = 1280 / DIVISOR2;
static constexpr int REFRACTION_HEIGHT = 720 / DIVISOR2;

WaterFrameBuffers::WaterFrameBuffers()
{
    initializeReflectionFrameBuffer();
    initializeRefractionFrameBuffer();
}

WaterFrameBuffers::~WaterFrameBuffers()
{
    glDeleteFramebuffers(1, &reflectionFrameBufferID);
    glDeleteTextures(1, &reflectionTextureID);
    glDeleteRenderbuffers(1, &reflectionDepthBufferID);
    glDeleteFramebuffers(1, &refractionFrameBufferID);
    glDeleteTextures(1, &refractionTextureID);
    glDeleteRenderbuffers(1, &refractionDepthBufferID);
    glDeleteTextures(1, &refractionDepthTextureID);
}

// call before rendering to this FBO
void WaterFrameBuffers::bindReflectionFrameBuffer()
{
    bindFrameBuffer(reflectionFrameBufferID, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

// call before rendering to this FBO
void WaterFrameBuffers::bindRefractionFrameBuffer()
{
    bindFrameBuffer(refractionFrameBufferID, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

// switch back to default framebuffer
void WaterFrameBuffers::unbindCurrentFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, VuWindow::getWidth(), VuWindow::getHeight());
}

void WaterFrameBuffers::initializeReflectionFrameBuffer()
{
    reflectionFrameBufferID = createFrameBuffer();
    reflectionTextureID = createTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
    reflectionDepthBufferID = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
    unbindCurrentFrameBuffer();
}

void WaterFrameBuffers::initializeRefractionFrameBuffer()
{
    refractionFrameBufferID = createFrameBuffer();
    refractionTextureID = createTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
    refractionDepthBufferID = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
    refractionDepthTextureID = createDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
    unbindCurrentFrameBuffer();
}

void WaterFrameBuffers::bindFrameBuffer(unsigned int framebufferID, int width, int height)
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    glViewport(0, 0, width, height);
}

unsigned int WaterFrameBuffers::createFrameBuffer()
{
    unsigned int framebufferID;
    glGenFramebuffers(1, &framebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    return framebufferID;
}

unsigned int WaterFrameBuffers::createTextureAttachment(int width, int height)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureID, 0);
    return textureID;
}

unsigned int WaterFrameBuffers::createDepthTextureAttachment(int width, int height)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureID, 0);
    return textureID;
}

unsigned int WaterFrameBuffers::createDepthBufferAttachment(int width, int height)
{
    unsigned int depthbufferID;
    glGenRenderbuffers(1, &depthbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, depthbufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthbufferID);
    return depthbufferID;
}
