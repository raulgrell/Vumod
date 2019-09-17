#pragma once

#include <math/Math.h>

class Camera
{
public:
    Camera() = default;
    Camera(const Mat4 &projectionMatrix): projectionMatrix(projectionMatrix){}

    void Update();

    Mat4 GetViewMatrix() const { return viewMatrix; };
    Vec3 getPosition() const { return position; }
    Vec3 getRotation() const { return rotation; }

    static Camera Orthographic()
    {
        Camera cam;
        return cam;
    };

    static Camera Perspective()
    {
        Camera cam;
        return cam;
    };

public:
    inline static float fov = 70;
    inline static float near_plane = 0.1f;
    inline static float far_plane = 10000.0f;

public:
    Vec3 position {0};
    Vec3 rotation {0};
    Mat4 projectionMatrix {0};
    Mat4 viewMatrix {0};
};
