#pragma once

#include <math/Math.h>
#include <terrain/Terrain.h>
#include "camera/Camera.h"

class MousePicker
{
    static constexpr int RECURSION_COUNT = 200;
	static constexpr float RAY_RANGE = 1000;

public:
	MousePicker(Camera& camera, Mat4& projection, Terrain* terrain);

	Vec3* GetCurrentTerrainPoint() {
		if (currentTerrainPointIsOk) {
			return &currentTerrainPoint;
		} else {
			return nullptr;
		}
	}

	Vec3& GetCurrentRay() {
		return currentRay;
	}

	void Update();

private:
	Vec3 CalculateMouseRay();
	Vec3 ToWorldCoords(Vec4 eyeCoords);
	Vec4 ToEyeCoords(Vec4 clipCoords);
	Vec2 GetNormalizedDeviceCoords(float mouseX, float mouseY);
	Vec3 GetPointOnRay(Vec3 &ray, float distance);
	Vec3 BinarySearch(int count, float start, float finish, Vec3 &ray, bool &ok);
	bool IntersectionInRange(float start, float finish, Vec3 &ray);
	bool IsUnderGround(Vec3 &testPoint);
	Terrain* GetTerrain(float worldX, float worldZ);

	Vec3 currentRay;
	Mat4 projectionMatrix;
	Mat4 viewMatrix;
	Camera& camera;
	Terrain* terrain;
	Vec3 currentTerrainPoint;
	bool currentTerrainPointIsOk;
};
