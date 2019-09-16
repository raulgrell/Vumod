#pragma once

#include <math/Mat.h>

class ICamera
{
    virtual Mat4 GetCameraMatrix() = 0;
    virtual Mat4 GetProjectionMatrix() = 0;
    virtual Mat4 GetViewMatrix() = 0;
};
