#include "AnimatedModel.h"

AnimatedModel::AnimatedModel(const VertexArray &model, const VuTexture &texture, Joint &rootJoint, int jointCount)
        : model(model), texture(texture), rootJoint(rootJoint), jointCount(jointCount),
          animator(std::make_unique<Animator>()) {
    mat4x4 r;
    rootJoint.calcInverseBindTransform(r);
}

void AnimatedModel::doAnimation(Animation animation)
{
    animator->doAnimation(animation);
}

void AnimatedModel::update()
{
    animator->update();
}

std::vector<mat4x4> AnimatedModel::getJointTransforms()
{
    std::vector<mat4x4> jointMatrices(jointCount);
    addJointsToArray(rootJoint, jointMatrices);
    return jointMatrices;
}

void AnimatedModel::addJointsToArray(const Joint &headJoint, const std::vector<mat4x4> &jointMatrices)
{
    jointMatrices[headJoint.index] = headJoint.getAnimatedTransform();
    for (Joint childJoint : headJoint.children) {
        addJointsToArray(childJoint, jointMatrices);
    }
}
