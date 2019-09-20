
#include <platform/VuWindow.h>
#include "MousePicker.h"

MousePicker::MousePicker(Camera& camera, Mat4& projection, Terrain* terrain)
        : camera(camera), projectionMatrix(projection), terrain(terrain)
{
	viewMatrix = camera.GetViewMatrix();
	currentTerrainPointIsOk = false;
}

void MousePicker::Update()
{
	viewMatrix = camera.GetViewMatrix();
	currentRay = CalculateMouseRay();
	bool ok = false;
	if (IntersectionInRange(0, RAY_RANGE, currentRay)) {
		currentTerrainPoint = BinarySearch(0, 0, RAY_RANGE, currentRay, ok);
		currentTerrainPointIsOk = ok;
	} else {
		currentTerrainPoint = Vec3(0, 0, 0);
		currentTerrainPointIsOk = false;
	}
}

Vec3 MousePicker::CalculateMouseRay()
{
	// viewport space
	float mouseX = 0;// mouse.getX();
	float mouseY = 0;// mouse.getY();
	// normalized device space
	Vec2 normalizedCoords = GetNormalizedDeviceCoords(mouseX, mouseY);
	// homogenous clip space
	Vec4 clipCoords = Vec4(normalizedCoords.x, normalizedCoords.y, -1.f, 1.f);
	// eye space
	Vec4 eyeCoords = ToEyeCoords(clipCoords);
	// world space
	Vec3 worldRay = ToWorldCoords(eyeCoords);
	return worldRay;
}

Vec3 MousePicker::ToWorldCoords(Vec4 eyeCoords)
{
	Mat4 invertedView = Mat4::Inverse(viewMatrix);
	Vec4 rayWorld = invertedView * eyeCoords;
	Vec3 mouseRay = Vec3(rayWorld.x, rayWorld.y, rayWorld.z);
	mouseRay = Vec3::Normal(mouseRay);
	return mouseRay;
}

Vec4 MousePicker::ToEyeCoords(Vec4 clipCoords)
{
	Mat4 invertedProjection = Mat4::Inverse(projectionMatrix);
	Vec4 eyeCoords = invertedProjection * clipCoords;
	return Vec4(eyeCoords.x, eyeCoords.y, -1.f, 0.f);
}

// convert to opengl coordinate system -1 -> 1, -1 -> 1,
// origin at the center of screen
Vec2 MousePicker::GetNormalizedDeviceCoords(float mouseX, float mouseY)
{
	float x = (2 * mouseX) / VuWindow::width - 1;
	float y = (2 * mouseY) / VuWindow::height - 1;
	return Vec2(x, -y);
}

Vec3 MousePicker::GetPointOnRay(Vec3 &ray, float distance)
{
	Vec3 start = camera.GetPosition();
	Vec3 scaledRay = Vec3(ray.x * distance, ray.y * distance, ray.z * distance);
	return Vec3(start.x + scaledRay.x, start.y + scaledRay.y, start.z + scaledRay.z);
}

Vec3 MousePicker::BinarySearch(int count, float start, float finish, Vec3 &ray, bool &ok)
{
	float half = start + ((finish - start) / 2.0f);
	if (count >= RECURSION_COUNT) {
		Vec3 endPoint = GetPointOnRay(ray, half);

		Terrain* tile = GetTerrain(endPoint.x, endPoint.z);
        ok = tile != nullptr;
		return endPoint;
	}
	if (IntersectionInRange(start, half, ray)) {
		return BinarySearch(count + 1, start, half, ray, ok);
	} else {
		return BinarySearch(count + 1, half, finish, ray, ok);
	}
}

bool MousePicker::IntersectionInRange(float start, float finish, Vec3 &ray)
{
	Vec3 startPoint = GetPointOnRay(ray, start);
	Vec3 endPoint = GetPointOnRay(ray, finish);
    return !IsUnderGround(startPoint) && IsUnderGround(endPoint);
}

bool MousePicker::IsUnderGround(Vec3 &testPoint)
{
	Terrain* tile = GetTerrain(testPoint.x, testPoint.z);
	float height = 0;
	if (tile != nullptr) {
		height = tile->GetHeightOfTerrain(testPoint.x, testPoint.z);
	}
    return testPoint.y < height;
}

Terrain* MousePicker::GetTerrain(float worldX, float worldZ)
{
	return terrain;
}
