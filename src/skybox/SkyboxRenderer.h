#pragma once

#include "graphics/Loader.h"
#include "SkyboxShader.h"

class Camera;

class SkyboxRenderer {
public:
	SkyboxRenderer(Loader& loader, Mat4& projectionMatrix);
	void Render(Camera &camera, float r, float g, float b);
private:
	void BindTextures();

	static constexpr float SIZE = 5000.0f;
	std::vector<float> VERTICES = {
		-SIZE, SIZE,-SIZE,
		-SIZE,-SIZE,-SIZE,
		 SIZE,-SIZE,-SIZE,
		 SIZE,-SIZE,-SIZE,
		 SIZE, SIZE,-SIZE,
		-SIZE, SIZE,-SIZE,

		-SIZE,-SIZE, SIZE,
		-SIZE,-SIZE,-SIZE,
		-SIZE, SIZE,-SIZE,
		-SIZE, SIZE,-SIZE,
		-SIZE, SIZE, SIZE,
		-SIZE,-SIZE, SIZE,

		 SIZE,-SIZE,-SIZE,
		 SIZE,-SIZE, SIZE,
		 SIZE, SIZE, SIZE,
		 SIZE, SIZE, SIZE,
		 SIZE, SIZE,-SIZE,
		 SIZE,-SIZE,-SIZE,

		-SIZE,-SIZE, SIZE,
		-SIZE, SIZE, SIZE,
		 SIZE, SIZE, SIZE,
		 SIZE, SIZE, SIZE,
		 SIZE,-SIZE, SIZE,
		-SIZE,-SIZE, SIZE,

		-SIZE, SIZE,-SIZE,
		 SIZE, SIZE,-SIZE,
		 SIZE, SIZE, SIZE,
		 SIZE, SIZE, SIZE,
		-SIZE, SIZE, SIZE,
		-SIZE, SIZE,-SIZE,

		-SIZE,-SIZE,-SIZE,
		-SIZE,-SIZE, SIZE,
		 SIZE,-SIZE,-SIZE,
		 SIZE,-SIZE,-SIZE,
		-SIZE,-SIZE, SIZE,
		 SIZE,-SIZE, SIZE
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

	RawModel cube;
	unsigned int texture;
	unsigned int nightTexture;
	SkyboxShader shader;
	float myTime = 5000;
};

