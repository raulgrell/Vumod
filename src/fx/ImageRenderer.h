#pragma once

#include <graphics/FrameBuffer.h>

class ImageRenderer
{
    FrameBuffer fbo;

public:
    ImageRenderer(int width, int height)
            : fbo(width, height, FrameBuffer::NONE)
    {
    }

    void RenderQuad()
    {
        fbo.Bind();
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        fbo.Unbind();
    }

    int GetOutputTexture() const { return fbo.GetColourTexture(); }
};
