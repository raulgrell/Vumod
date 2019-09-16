#pragma once

#include <math/Math.h>
#include <terrain/Terrain.h>
#include "scene/Camera.h"

class MousePicker
{
public:
	MousePicker(Camera& camera, Mat4& projection, Terrain* terrain);

	Vec3* getCurrentTerrainPoint() {
		if (currentTerrainPointIsOK) {
			return &currentTerrainPoint;
		} else {
			return nullptr;
		}
	}
	Vec3& getCurrentRay() {
		return currentRay;
	}
	void update();
private:
	Vec3 calculateMouseRay();
	Vec3 toWorldCoords(Vec4 eyeCoords);
	Vec4 toEyeCoords(Vec4 clipCoords);
	Vec2 getNormalizedDeviceCoords(float mouseX, float mouseY);
	Vec3 getPointOnRay(Vec3& ray, float distance);
	Vec3 binarySearch(int count, float start, float finish, Vec3& ray, bool& ok);
	bool intersectionInRange(float start, float finish, Vec3& ray);
	bool isUnderGround(Vec3& testPoint);
	Terrain* getTerrain(float worldX, float worldZ);

	static constexpr int RECURSION_COUNT = 200;
	static constexpr float RAY_RANGE = 1000; // 600;

	Vec3 currentRay;
	Mat4 projectionMatrix;
	Mat4 viewMatrix;
	Camera& camera;
	Terrain* terrain;
	Vec3 currentTerrainPoint;
	bool currentTerrainPointIsOK;
};
