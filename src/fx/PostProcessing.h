
#pragma once

#include <models/Model.h>
#include "ContrastShader.h"

class PostProcessing {
	inline static float POSITIONS[] = { -1, 1, -1, -1, 1, 1, 1, -1 };

	inline static std::unique_ptr<RawModel> quad;
	inline static std::unique_ptr<ContrastChanger> contrastChanger;

	static void Begin();
	static void End();

public:
	static void Init();
	static void Render(int colourTexture);
};
