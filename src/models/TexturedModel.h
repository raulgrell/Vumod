#pragma once

#include <graphics/TextureData.h>
#include "RawModel.h"

struct TexturedModel
{
    RawModel &rawModel;
    ModelTexture &texture;

    TexturedModel(RawModel &model, ModelTexture &texture)
            : rawModel(model), texture(texture) {}

    ModelTexture &getTexture()
    {
        return texture;
    }
};
