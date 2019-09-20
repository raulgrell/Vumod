#pragma once

#include <shaders/ShaderLibrary.h>

class AnimatedModelShader : Shader {
	constexpr static int MAX_JOINTS = 50;
	constexpr static int DIFFUSE_TEX_UNIT = 0;

	constexpr static const char *SHADER_FILE = "shader/animated.glsl";

	int u_projectionViewMatrix = 0;
	int u_lightDirection = 0;
	int u_jointTransforms = 0;
	int u_diffuseMap = 0;

public:
    AnimatedModelShader() : Shader(VERTEX_SHADER, FRAGMENT_SHADER) {
		super.storeAllUniformLocations(projectionViewMatrix, diffuseMap, lightDirection, jointTransforms);
		connectTextureUnits();
	}

	void connectTextureUnits() {
		super.start();
		diffuseMap.loadTexUnit(DIFFUSE_TEX_UNIT);
		super.stop();
	}

	void Bind() const {

    }

	void Unbind() const {

    }

	const std::string& GetName() const {

    }
};
