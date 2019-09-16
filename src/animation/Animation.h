#pragma once

#include <common.h>

#include <math/Quat.h>
#include <math/Vec.h>
#include <math/Mat.h>

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

class Joint
{
public:
    int index;
    std::string name;
    std::vector<const Joint&> children;

private:
    mat4x4 animatedTransform {0};
    mat4x4 localBindTransform {0};
    mat4x4 inverseBindTransform {0};

public:
    Joint(int index, std::string name, mat4x4 localBindTransform)
        :index(index), name(std::move(name)) {
        mat4x4_dup(this->localBindTransform, localBindTransform);
    }

    void AddChild(const Joint &child) {
        children.push_back(child);
    }

    void calcInverseBindTransform(mat4x4 parentBindTransform) {
        mat4x4 bindTransform;
        mat4x4_mul(bindTransform, parentBindTransform, localBindTransform);
        mat4x4_invert(bindTransform, inverseBindTransform);
		for (Joint child : children) {
			child.calcInverseBindTransform(bindTransform);
		}
	}
};

struct JointTransform
{
	Vec3 position;
	Quat rotation;

    JointTransform(Vec3 position, Quat rotation)
	    : position(position), rotation(rotation) {
	}

    Mat4 GetLocalTransform() {
		Mat4 matrix;
		matrix.translate(position);
		Mat4.mul(matrix, rotation.toRotationMatrix(), matrix);
		return matrix;
	}

	static JointTransform interpolate(JointTransform frameA, JointTransform frameB, float progression) {
		Vec3 pos = interpolate(frameA.position, frameB.position, progression);
		Quat rot = Quat::nlerp(frameA.rotation, frameB.rotation, progression);
		return JointTransform(pos, rot);
	}

	static Vec3 interpolate(Vec3 start, Vec3 end, float progression) {
		float x = start.x + (end.x - start.x) * progression;
		float y = start.y + (end.y - start.y) * progression;
		float z = start.z + (end.z - start.z) * progression;
		return Vec3{x, y, z};
	}
};


class KeyFrame
{
	float timeStamp;
	std::unordered_map<std::string, JointTransform> pose;

	KeyFrame(float timeStamp, std::unordered_map<std::string, JointTransform> jointKeyFrames)
	    : timeStamp(timeStamp), pose(std::move(jointKeyFrames)) {
	}
};

class Animation
{
public:
    float length;
    KeyFrame *keyFrames;

	Animation(float lengthInSeconds, KeyFrame *frames)
	    : length(lengthInSeconds), keyFrames(frames) { }
};

