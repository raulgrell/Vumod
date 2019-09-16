#pragma once

#include <math/Mat.h>
#include "TerrainShader.h"
#include "Terrain.h"

class TerrainRenderer {
public:
	TerrainRenderer(TerrainShader& shader, Mat4& projectionMatrix);
	void Render(std::vector<Terrain *> *terrains);
	void Prepare(Terrain& terrain);
	void BindTextures(Terrain &terrain);
	void LoadModelMatrix(Terrain& terrain);
	void UnbindTexturedModel();
private:
	TerrainShader& shader;
};

