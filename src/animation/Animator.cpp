#include "Animator.h"

float Animator::calculateProgression(KeyFrame previousFrame, KeyFrame nextFrame)
{
    float totalTime = nextFrame.getTimeStamp() - previousFrame.getTimeStamp();
    float currentTime = animationTime - previousFrame.getTimeStamp();
    return currentTime / totalTime;
}

std::unordered_map<std::string, mat4x4>
Animator::interpolatePoses(KeyFrame previousFrame, KeyFrame nextFrame, float progression)
{
    Map<String, Matrix4f> currentPose = new HashMap<String, Matrix4f>();
    for (String jointName : previousFrame.getJointKeyFrames().keySet()) {
        JointTransform previousTransform = previousFrame.getJointKeyFrames().get(jointName);
        JointTransform nextTransform = nextFrame.getJointKeyFrames().get(jointName);
        JointTransform currentTransform = JointTransform.interpolate(previousTransform, nextTransform, progression);
        currentPose.put(jointName, currentTransform.getLocalTransform());
    }
    return currentPose;
}

int Animator::getPreviousAndNextFrames()
{
    KeyFrame[] allFrames = currentAnimation.getKeyFrames();
    KeyFrame previousFrame = allFrames[0];
    KeyFrame nextFrame = allFrames[0];
    for (int i = 1; i < allFrames.length; i++) {
        nextFrame = allFrames[i];
        if (nextFrame.getTimeStamp() > animationTime) {
            break;
        }
        previousFrame = allFrames[i];
    }
    return new KeyFrame[] { previousFrame, nextFrame };
}

void applyPoseToJoints(std::unordered_map<std::string, mat4x4> currentPose, const Joint& joint, Mat4 parentTransform)
{
    Matrix4f currentLocalTransform = currentPose.get(joint.name);
    Matrix4f currentTransform = Matrix4f.mul(parentTransform, currentLocalTransform, null);
    for (Joint childJoint : joint.children) {
        applyPoseToJoints(currentPose, childJoint, currentTransform);
    }
    Matrix4f.mul(currentTransform, joint.getInverseBindTransform(), currentTransform);
    joint.setAnimationTransform(currentTransform);
}

std::unordered_map<std::string, mat4x4> Animator::calculateCurrentAnimationPose()
{
    KeyFrame[] frames = getPreviousAndNextFrames();
    float progression = calculateProgression(frames[0], frames[1]);
    return interpolatePoses(frames[0], frames[1], progression);
}

void Animator::increaseAnimationTime(float dt)
{
    animationTime += dt;
    if (animationTime > currentAnimation->length) {
        animationTime = animationTime - currentAnimation->length;
    }
}

void Animator::update()
{
    if (currentAnimation == nullptr)
        return;

    increaseAnimationTime();
    auto currentPose = calculateCurrentAnimationPose();
    applyPoseToJoints(currentPose, entity.getRootJoint(), new Matrix4f());
}

void Animator::doAnimation(Animation &animation)
{
    animationTime = 0;
    currentAnimation = &animation;
}
