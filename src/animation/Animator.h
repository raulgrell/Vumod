#pragma once

#include "Animation.h"
#include "AnimatedModel.h"

class Animator
{
    const AnimatedModel &entity;

    Animation *currentAnimation = nullptr;
    float animationTime = 0;

    Animator(const AnimatedModel &entity)
        : entity(entity)
    {
    }

    void increaseAnimationTime(float dt);

    KeyFrame[] getPreviousAndNextFrames();
    float calculateProgression(KeyFrame previousFrame, KeyFrame nextFrame);

    std::unordered_map<std::string, mat4x4> calculateCurrentAnimationPose();
    std::unordered_map<std::string, mat4x4> interpolatePoses(KeyFrame previousFrame, KeyFrame nextFrame, float progression);

    void applyPoseToJoints(std::unordered_map<std::string, mat4x4> currentPose, const Joint& joint, Mat4 parentTransform);

public:
    void doAnimation(Animation &animation);
    void update();
};

