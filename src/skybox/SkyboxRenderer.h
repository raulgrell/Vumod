#pragma once

#include "graphics/Loader.h"
#include "SkyboxShader.h"

class Camera;

class SkyboxRenderer {
public:
	SkyboxRenderer(Mat4 &projectionMatrix);
	void Render(Camera &camera, float r, float g, float b);

private:
	void BindTextures();

private:
	static constexpr float SIZE = 5000.0f;

	std::unique_ptr<IndexedModel>    cube;
	unsigned int texture;
	unsigned int nightTexture;
	SkyboxShader shader;
	float myTime = 5000;

	std::vector<unsigned int> INDICES = {
	        0, 1, 2, 2, 3, 0,
	        4, 5, 6, 6, 7, 4,
	        8, 9, 10, 10, 11, 8,
	        12, 13, 14, 14, 15, 12,
	        16, 17, 18, 18, 19, 16,
	        20, 21, 22, 22, 23, 20
	};

	std::vector<float> VERTICES = {
		-SIZE, SIZE,-SIZE,
		-SIZE,-SIZE,-SIZE,
		 SIZE,-SIZE,-SIZE,
		 SIZE, SIZE,-SIZE,

		-SIZE,-SIZE, SIZE,
		-SIZE,-SIZE,-SIZE,
		-SIZE, SIZE,-SIZE,
		-SIZE, SIZE, SIZE,

		 SIZE,-SIZE,-SIZE,
		 SIZE,-SIZE, SIZE,
		 SIZE, SIZE, SIZE,
		 SIZE, SIZE,-SIZE,

		-SIZE,-SIZE, SIZE,
		-SIZE, SIZE, SIZE,
		 SIZE, SIZE, SIZE,
		 SIZE,-SIZE, SIZE,

		-SIZE, SIZE,-SIZE,
		 SIZE, SIZE,-SIZE,
		 SIZE, SIZE, SIZE,
		-SIZE, SIZE, SIZE,

		-SIZE,-SIZE,-SIZE,
		-SIZE,-SIZE, SIZE,
		 SIZE,-SIZE,-SIZE,
		-SIZE,-SIZE, SIZE,
	};

	std::vector<std::string> TEXTURE_FILES = {
		"data/res/skyRight",
		"data/res/skyLeft",
		"data/res/skyTop",
		"data/res/skyBottom",
		"data/res/skyBack",
		"data/res/skyFront"
	};

	std::vector<std::string> NIGHT_TEXTURE_FILES = {
		"data/res/nightRight",
		"data/res/nightLeft",
		"data/res/nightTop",
		"data/res/nightBottom",
		"data/res/nightBack",
		"data/res/nightFront"
	};
};

