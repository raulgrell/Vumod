#include "WaterTile.h"

WaterTile::WaterTile(float centerX, float centerZ, float height, float scale)
{
	this->x = centerX;
	this->z = centerZ;
	this->height = height;
	this->scale = scale;
}

float WaterTile::getHeight()
{
	return height;
}

float WaterTile::getX()
{
	return x;
}

float WaterTile::getZ()
{
	return z;
}

float WaterTile::getScale()
{
	return scale;
}
