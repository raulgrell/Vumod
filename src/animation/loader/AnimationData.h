#pragma once

#include "KeyFrameData.h"

struct AnimationData {
	float lengthSeconds;
	KeyFrameData *keyFrames;

	AnimationData(float lengthSeconds, KeyFrameData *keyFrames)
	    : lengthSeconds(lengthSeconds), keyFrames(keyFrames) {
	}
};

struct JointData
{
	int index;
	std::string nameId;
	Mat4 bindLocalTransform;

	std::vector<JointData> children;

	JointData(int index, std::string nameId, Mat4 bindLocalTransform)
	    : index(index), nameId(nameId), bindLocalTransform(bindLocalTransform) {
	}

	void addChild(JointData child) {
		children.add(child);
	}
}

struct JointTransformationData
{
	std::string jointNameId;
	Mat4 jointLocalTransform;

	JointTransformData(std::string jointNameId, Mat4 jointLocalTransform) {
		this.jointNameId = jointNameId;
		this.jointLocalTransform = jointLocalTransform;
	}
}

struct KeyFrameData
{
	float time;
	std::vector<JointTransformData> jointTransforms;

	KeyFrameData(float time){
		this.time = time;
	}

	void addJointTransform(JointTransformData transform){
		jointTransforms.add(transform);
	}
}