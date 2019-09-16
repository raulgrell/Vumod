#pragma once

#include <interfaces/Renderer.h>
#include "AnimatedModelShader.h"
#include "AnimatedModel.h"

class AnimatedModelRenderer : Renderer
{
	AnimatedModelShader shader;

public:
    AnimatedModelRenderer() : shader(std::make_unique<AnimatedModelShader>())
    {

    }

	void render(AnimatedModel entity, ICamera camera, Vector3f lightDir);

	void prepare(ICamera camera, Vector3f lightDir);


};

