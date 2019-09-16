#pragma once

class WaterFrameBuffers {
public:
	WaterFrameBuffers();
	~WaterFrameBuffers();

	void bindReflectionFrameBuffer();
	void bindRefractionFrameBuffer();
	void unbindCurrentFrameBuffer();
	unsigned int getReflectionTexture() { return reflectionTextureID; }
	unsigned int getRefractionTexture() { return refractionTextureID; }
	unsigned int getRefractionDepthTexture() { return refractionDepthTextureID; }
	
private:
	unsigned int reflectionFrameBufferID = 0;
	unsigned int reflectionTextureID = 0;
	unsigned int reflectionDepthBufferID = 0;
	
	unsigned int refractionFrameBufferID = 0;
	unsigned int refractionTextureID = 0;
	unsigned int refractionDepthBufferID = 0;
	unsigned int refractionDepthTextureID = 0;
	
	void initializeRefractionFrameBuffer();
	void initializeReflectionFrameBuffer();
	void bindFrameBuffer(unsigned int framebufferID, int width, int height);
	unsigned int createFrameBuffer();
	unsigned int createTextureAttachment(int width, int height);
	unsigned int createDepthTextureAttachment(int width, int height);
	unsigned int createDepthBufferAttachment(int width, int height);
};
