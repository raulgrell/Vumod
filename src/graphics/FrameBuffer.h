#pragma once

#include <glad/glad.h>

class FrameBuffer {
	int width;
	int height;
	unsigned int frameBuffer = 0;
	unsigned int colourTexture = 0;
	unsigned int depthTexture = 0;
	unsigned int depthBuffer = 0;
	unsigned int colourBuffer = 0;

public:
    inline static unsigned int NONE = 0;
    inline static unsigned int DEPTH_TEXTURE = 1;
	inline static unsigned int DEPTH_RENDER_BUFFER = 2;

    FrameBuffer(int width, int height, unsigned int depthBufferType);
    ~FrameBuffer();

	void Bind();
	void Unbind();

	void BindToRead();

	int GetColourTexture() const { return colourTexture; }
	int GetDepthTexture() const { return depthTexture; }

	void CreateFrameBuffer();
	void CreateTextureAttachment();
	void CreateDepthTextureAttachment();
	void CreateDepthBufferAttachment();
};


