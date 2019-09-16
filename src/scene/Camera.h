#pragma once

#include <math/Math.h>

class Camera
{
public:
    Camera() = default;

    void Update();

    Mat4 GetViewMatrix() const;
    Vec3 getPosition() const { return position; }

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

    Vec3 position;
    Vec3 rotation;
};
