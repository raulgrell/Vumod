#pragma once

class WaterFrameBuffers {
public:
	WaterFrameBuffers();
	~WaterFrameBuffers();

	void BindReflectionFrameBuffer();
	void BindRefractionFrameBuffer();
	unsigned int GetReflectionTexture() { return reflectionTextureId; }
	unsigned int GetRefractionTexture() { return refractionTextureId; }
	unsigned int GetRefractionDepthTexture() { return refractionDepthTextureId; }

	static void UnbindCurrentFrameBuffer();

private:
	unsigned int reflectionFrameBufferId = 0;
	unsigned int reflectionTextureId = 0;
	unsigned int reflectionDepthBufferId = 0;
	
	unsigned int refractionFrameBufferId = 0;
	unsigned int refractionTextureId = 0;
	unsigned int refractionDepthBufferId = 0;
	unsigned int refractionDepthTextureId = 0;
	
	void InitializeRefractionFrameBuffer();
	void InitializeReflectionFrameBuffer();

	static void BindFrameBuffer(unsigned int framebufferId, int width, int height);
	static unsigned int CreateFrameBuffer();
	static unsigned int CreateTextureAttachment(int width, int height);
	static unsigned int CreateDepthTextureAttachment(int width, int height);
	static unsigned int CreateDepthBufferAttachment(int width, int height);
};
