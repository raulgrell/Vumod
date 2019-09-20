#pragma once

class WaterTile {

public:
	WaterTile(float centerX, float centerZ, float height, float scale)
	    : x(centerX), z(centerZ), height(height), scale(scale) {}

    float GetHeight() { return height; }
    float GetX() { return x; }
    float GetZ() { return z; }
    float GetScale() { return scale; }

private:
	float x, z;
	float height;
	float scale;

	static constexpr float DEFAULT_TILE_SCALE = 4*120;
	static constexpr float DEFAULT_TILE_SIZE = DEFAULT_TILE_SCALE * 2;
};




