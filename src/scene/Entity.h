#pragma once

#include <math/Vec.h>
#include <models/Model.h>

struct Entity
{
    TexturedModel &model;
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
    int atlasIndex = 0;

    Entity(TexturedModel &model,
           Vec3 position,
           Vec3 rotation,
           Vec3 scale)
            : model(model),
              position(position),
              rotation(rotation),
              scale(scale)
    {
    }

    Entity(TexturedModel &model,
           int atlasIndex,
           Vec3 position,
           Vec3 rotation,
           Vec3 scale)
            : model(model),
              position(position),
              rotation(rotation),
              scale(scale),
              atlasIndex(atlasIndex)
    {
    }

    float GetTextureXOffset();
    float GetTextureYOffset();
};

