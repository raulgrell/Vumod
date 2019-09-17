#include "Entity.h"

float Entity::getTextureXOffset()
{
    int rows = model.texture.numberOfRows;
    int column = atlasIndex % rows;
    return (float) column / (float) rows;
}

float Entity::getTextureYOffset()
{
    int rows = model.texture.numberOfRows;
    int row = atlasIndex / rows;
    return (float) row / (float) rows;
}
