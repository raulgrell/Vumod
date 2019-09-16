#pragma once

#include <common.h>
#include <math/Vec.h>
#include "Colour.h"

struct DirectionalLight
{
    Vec3 direction;
    Colour colour;
    Vec2 lightBias;

public:
    DirectionalLight(Vec3 direction, Colour colour, Vec2 lightBias)
            : direction(direction), colour(colour), lightBias(lightBias)
    {
        this->direction.Normalize();
    }
};

struct Light
{
    Vec3 position;
    Vec3 color;
    Vec3 attenuation;

    Light(Vec3 position, Vec3 color)
            : position(position), color(color), attenuation(1.0, 0, 0) {}

    Light(Vec3 position, Vec3 color, Vec3 attenuation)
            : position(position), color(color), attenuation(attenuation)
    {
    }
};
