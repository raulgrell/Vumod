#pragma once

#include <graphics/Loader.h>
#include "WaterShader.h"
#include "WaterTile.h"
#include "WaterFrameBuffers.h"

class WaterRenderer {
public:
    WaterRenderer(WaterShader &shader,
                  Mat4 &projectionMatrix,
                  float nearPlane,
                  float farPlane,
                  WaterFrameBuffers &fbos);

	void Render(std::vector<WaterTile> &water, Camera &camera, Light &sun);

	void Prepare(Camera &camera, Light &sun);

	void Unbind();

	void SetUpVao();

private:
	std::unique_ptr<IndexedModel> quad;
	WaterShader& shader;
	WaterFrameBuffers& fbos;
    unsigned int dudvTexture;
    unsigned int normalMap;

	float moveFactor = 0;
	float waterTiling = 20.0f;
	float waveStrength = 0.04f;
	float waterReflectivity = 0.5f;
	float shineDamper = 20.0f;
	float reflectivity = 0.5f;
	float waveSpeed = 0.03f;
};

