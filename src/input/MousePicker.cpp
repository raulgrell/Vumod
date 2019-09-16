
#include <platform/VuWindow.h>
#include "MousePicker.h"

MousePicker::MousePicker(Camera& camera, Mat4& projection, Terrain* terrain)
        : camera(camera), projectionMatrix(projection), terrain(terrain)
{
	this->viewMatrix = camera.GetViewMatrix();
	this->currentTerrainPointIsOK = false;
}

void MousePicker::update()
{
	viewMatrix = camera.GetViewMatrix();
	currentRay = calculateMouseRay();
	bool ok = false;
	if (intersectionInRange(0, RAY_RANGE, currentRay)) {
		currentTerrainPoint = binarySearch(0, 0, RAY_RANGE, currentRay, ok);
		currentTerrainPointIsOK = ok;
	} else {
		currentTerrainPoint = Vec3(0, 0, 0);
		currentTerrainPointIsOK = false;
	}
}

Vec3 MousePicker::calculateMouseRay()
{
	// viewport space
	float mouseX = 0;// mouse.getX();
	float mouseY = 0;// mouse.getY();
	// normalized device space
	Vec2 normalizedCoords = getNormalizedDeviceCoords(mouseX, mouseY);
	// homogenous clip space
	Vec4 clipCoords = Vec4(normalizedCoords.x, normalizedCoords.y, -1.f, 1.f);
	// eye space
	Vec4 eyeCoords = toEyeCoords(clipCoords);
	// world space
	Vec3 worldRay = toWorldCoords(eyeCoords);
	return worldRay;
}

Vec3 MousePicker::toWorldCoords(Vec4 eyeCoords)
{
	Mat4 invertedView = Mat4::Inverse(this->viewMatrix);
	Vec4 rayWorld = invertedView * eyeCoords;
	Vec3 mouseRay = Vec3(rayWorld.x, rayWorld.y, rayWorld.z);
	mouseRay = Vec3::Normal(mouseRay);
	return mouseRay;
}

Vec4 MousePicker::toEyeCoords(Vec4 clipCoords)
{
	Mat4 invertedProjection = Mat4::Inverse(this->projectionMatrix);
	Vec4 eyeCoords = invertedProjection * clipCoords;
	return Vec4(eyeCoords.x, eyeCoords.y, -1.f, 0.f);
}

// convert to opengl coordinate system -1 -> 1, -1 -> 1,
// origin at the center of screen
Vec2 MousePicker::getNormalizedDeviceCoords(float mouseX, float mouseY)
{
	float x = (2 * mouseX) / VuWindow::width - 1;
	float y = (2 * mouseY) / VuWindow::height - 1;
	return Vec2(x, -y);
}

Vec3 MousePicker::getPointOnRay(Vec3& ray, float distance)
{
	Vec3 start = camera.getPosition();
	Vec3 scaledRay = Vec3(ray.x * distance, ray.y * distance, ray.z * distance);
	return Vec3(start.x + scaledRay.x, start.y + scaledRay.y, start.z + scaledRay.z);
}

Vec3 MousePicker::binarySearch(int count, float start, float finish, Vec3& ray, bool& ok)
{
	float half = start + ((finish - start) / 2.0f);
	if (count >= RECURSION_COUNT) {
		Vec3 endPoint = getPointOnRay(ray, half);

		Terrain* tile = getTerrain(endPoint.x, endPoint.z);
        ok = tile != nullptr;
		return endPoint;
	}
	if (intersectionInRange(start, half, ray)) {
		return binarySearch(count + 1, start, half, ray, ok);
	} else {
		return binarySearch(count + 1, half, finish, ray, ok);
	}
}

bool MousePicker::intersectionInRange(float start, float finish, Vec3& ray)
{
	Vec3 startPoint = getPointOnRay(ray, start);
	Vec3 endPoint = getPointOnRay(ray, finish);
    return !isUnderGround(startPoint) && isUnderGround(endPoint);
}

bool MousePicker::isUnderGround(Vec3& testPoint)
{
	Terrain* tile = getTerrain(testPoint.x, testPoint.z);
	float height = 0;
	if (tile != nullptr) {
		height = tile->getHeightOfTerrain(testPoint.x, testPoint.z);
	}
    return testPoint.y < height;
}

Terrain* MousePicker::getTerrain(float worldX, float worldZ)
{
	return terrain;
}
