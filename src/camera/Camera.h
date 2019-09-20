#pragma once

#include <math/Math.h>

class Camera
{
public:
    Camera() = default;
    Camera(const Mat4 &projectionMatrix): projectionMatrix(projectionMatrix){}

    void Update();

    Mat4 GetViewMatrix() const { return viewMatrix; };
    Vec3 GetPosition() const { return position; }
    Vec3 GetRotation() const { return rotation; }

    void InvertPitch() { rotation.x = -rotation.x; }

    static Camera Orthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane)
    {
        Camera cam(Mat4::Orthographic(left, right, bottom, top, nearPlane, farPlane));
        return cam;
    };

    static Camera Perspective(float fov, float aspectRatio, float nearPlane, float farPlane)
    {
        Camera cam(Mat4::Perspective(fov, aspectRatio, nearPlane, farPlane));
        return cam;
    };

public:
    inline static float fov = 70;
    inline static float aspectRatio = 70;
    inline static float nearPlane = 0.1f;
    inline static float farPlane = 10000.0f;

public:
    Vec3 position {0};
    Vec3 rotation {0};
    Mat4 projectionMatrix {0};
    Mat4 viewMatrix {0};

private:
    float width = 0;
    float height = 0;
};
