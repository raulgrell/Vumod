#include "Camera.h"

#include <cmath>
#include <math/Math.h>
#include <input/InputManager.h>
#include <GLFW/glfw3.h>

void Camera::Update()
{
    if (InputManager::IsKeyDown(GLFW_KEY_S)) {
        position += Vec3(0, 0, -1);
    }
    if (InputManager::IsKeyDown(GLFW_KEY_W)) {
        position += Vec3(0, 0, 1);
    }
    if (InputManager::IsKeyDown(GLFW_KEY_A)) {
        position += Vec3(-1, 0, 0);
    }
    if (InputManager::IsKeyDown(GLFW_KEY_D)) {
        position += Vec3(1, 0, 0);
    }
    if (InputManager::IsKeyDown(GLFW_KEY_SPACE)) {
        position += Vec3(0, 1, 0);
    }
    if (InputManager::IsKeyDown(GLFW_KEY_LEFT_CONTROL)) {
        position += Vec3(0, -1, 0);
    }
    if (InputManager::IsKeyDown(GLFW_KEY_Q)) {
        rotation.y += 1;
    }
    if (InputManager::IsKeyDown(GLFW_KEY_E)) {
        rotation.y -= 1;
    }

    viewMatrix = Mat4::Identity();
    viewMatrix *= projectionMatrix;
    viewMatrix *= Mat4::Rotate(math::radians(rotation.x), Vec3(1, 0, 0));
    viewMatrix *= Mat4::Rotate(math::radians(rotation.y), Vec3(0, 1, 0));
    viewMatrix *= Mat4::Translate(-position);
}
