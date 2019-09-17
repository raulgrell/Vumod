#pragma once

#include <math/Vec.h>

struct ModelVertex
{
    Vec3 position;
    Vec3 normal;
    Vec3 color;
    Vec2 uv;

    ModelVertex(const Vec3 &position, const Vec3 &normal, const Vec3 &color, const Vec2 &uv)
            : position(position), normal(normal), color(color), uv(uv) {}
};