#pragma once

#include <graphics/Loader.h>
#include "WaterShader.h"
#include "WaterTile.h"
#include "WaterFrameBuffers.h"

class WaterRenderer {
public:
    WaterRenderer(
            Loader& loader,
            WaterShader& shader,
            Mat4& projectionMatrix,
            float nearPlane,
            float farPlane,
            WaterFrameBuffers& fbos);

	void render(std::vector<WaterTile*>& water, Camera& camera, Light& sun);

	void prepareRender(Camera& camera, Light& sun);

	void unbind();

	void setUpVAO(Loader& loader);
private:
	RawModel quad;
	WaterShader& shader;
	WaterFrameBuffers& fbos;
    unsigned int dudvTexture;
    unsigned int normalMap;
	const std::string DUDV_MAP = "data/res/waterDUDV";
	const std::string NORMAL_MAP = "data/res/normalMap";

	float moveFactor = 0;
	float waterTiling = 20.0f;
	float waveStrength = 0.04f;
	float waterReflectivity = 0.5f;
	float shineDamper = 20.0f;
	float reflectivity = 0.5f;
	float waveSpeed = 0.03f;
};

