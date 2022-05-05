#include "FrameBuffer.h"

#include <glad/glad.h>
#include <platform/VuWindow.h>

FrameBuffer::FrameBuffer(int width, int height, unsigned int depthBufferType)
        : width(width), height(height) {
    CreateFrameBuffer();
    CreateTextureAttachment();
    if (depthBufferType == DEPTH_RENDER_BUFFER) {
        CreateDepthBufferAttachment();
    } else if (depthBufferType == DEPTH_TEXTURE) {
        CreateDepthTextureAttachment();
    }
    Unbind();
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &frameBuffer);
    glDeleteTextures(1, &colourTexture);
    glDeleteTextures(1, &depthTexture);
    glDeleteRenderbuffers(1, &depthBuffer);
    glDeleteRenderbuffers(1, &colourBuffer);
}

void FrameBuffer::Bind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, width, height);
}

void FrameBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, VuWindow::GetWidth(), VuWindow::GetHeight());
}

void FrameBuffer::CreateFrameBuffer()
{
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void FrameBuffer::CreateTextureAttachment()
{
    glGenTextures(1, &colourTexture);
    glBindTexture(GL_TEXTURE_2D, colourTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourTexture,
                           0);
}

void FrameBuffer::CreateDepthTextureAttachment()
{
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT,
                 GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
}

void FrameBuffer::CreateDepthBufferAttachment()
{
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                              depthBuffer);
}

void FrameBuffer::BindToRead()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
}
