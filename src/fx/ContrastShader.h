#pragma once

#include <shaders/ShaderGL.h>
#include "ImageRenderer.h"

class ContrastShader : public ShaderGL
{
public:
	inline static const char *SHADER_FILE = "data/shaders/contrast.glsl";

	ContrastShader() : ShaderGL(SHADER_FILE) {}

	void BindAttributes() override {
		BindAttribute(0, "position");
	}
};

class ContrastChanger {
    ImageRenderer renderer;
    ContrastShader shader;

public:
    ContrastChanger(int width, int height)
        : renderer(width, height), shader() {
    }

    void Render(int texture);
};
