#include "Entity.h"

float Entity::GetTextureXOffset()
{
    int rows = model.texture.GetNumberOfRows();
    int column = atlasIndex % rows;
    return (float) column / (float) rows;
}

float Entity::GetTextureYOffset()
{
    int rows = model.texture.GetNumberOfRows();
    int row = atlasIndex / rows;
    return (float) row / (float) rows;
}
