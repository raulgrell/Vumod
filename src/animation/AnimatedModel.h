#pragma once

#include <graphics/Buffer.h>
#include <graphics/VertexArray.h>
#include <texture/VuTexture.h>

#include "Joint.h"
#include "Animator.h"
#include "Animation.h"

class AnimatedModel
{
    const VertexArray& model;
    const VuTexture& texture;

    Joint& rootJoint;
    int jointCount;

    std::unique_ptr<Animator> animator;

    AnimatedModel(const VertexArray& model, const VuTexture& texture, Joint &rootJoint, int jointCount);


	void doAnimation(Animation animation);

	void update();

	std::vector<mat4x4> getJointTransforms();

	void addJointsToArray(const Joint& headJoint, const std::vector<mat4x4>& jointMatrices);
};