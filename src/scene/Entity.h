#pragma once

#include <models/TexturedModel.h>
#include <math/Vec.h>

struct Entity
{
    TexturedModel &model;
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
    int textureIndex = 0;

    Entity(TexturedModel &model,
           Vec3 position,
           Vec3 rotation,
           Vec3 scale)
            : model(model), position(position), rotation(rotation), scale(scale)
    {
    }

    Entity(TexturedModel &model,
           int textureIndex,
           Vec3 position,
           Vec3 rotation,
           Vec3 scale)
            : model(model), position(position), rotation(rotation), scale(scale), textureIndex(textureIndex)
    {
    }

    float getTextureXOffset();
    float getTextureYOffset();
};

