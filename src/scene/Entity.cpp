#include "Entity.h"

float Entity::getTextureXOffset()
{
    int rows = model.texture.numberOfRows;
    int column = textureIndex % rows;
    return (float) column / (float) rows;
}

float Entity::getTextureYOffset()
{
    int rows = model.texture.numberOfRows;
    int row = textureIndex / rows;
    return (float) row / (float) rows;
}
