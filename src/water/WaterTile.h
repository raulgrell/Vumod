#pragma once

class WaterTile {
public:
	static constexpr float DEFAULT_TILE_SCALE = 4*120;
	static constexpr float DEFAULT_TILE_SIZE = DEFAULT_TILE_SCALE * 2;

	WaterTile(float centerX, float centerZ, float height, float scale);
	float getHeight();
	float getX();
	float getZ();
	float getScale();
private:
	float height;
	float x, z;
	float scale;
};

