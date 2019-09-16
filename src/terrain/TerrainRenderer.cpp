#include "TerrainRenderer.h"

#include <glad/glad.h>
#include <scene/SceneShader.h>

TerrainRenderer::TerrainRenderer(TerrainShader& shader, Mat4& projectionMatrix) : shader(shader)
{
    shader.Bind();
	shader.LoadProjectionMatrix(projectionMatrix);
	CheckGL();
	shader.connectTextureUnits();
    shader.Unbind();
}

void TerrainRenderer::Render(std::vector<Terrain *> *terrains)
{
	for (auto terrain : *terrains) {
        Prepare(*terrain);
		LoadModelMatrix(*terrain);
	CheckGL();
		glDrawElements(GL_TRIANGLES, terrain->model.vertexCount, GL_UNSIGNED_INT, nullptr);
	CheckGL();
       UnbindTexturedModel();
	CheckGL();
	}
}

void TerrainRenderer::Prepare(Terrain& terrain)
{
	glBindVertexArray(terrain.model.vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	CheckGL();

    BindTextures(terrain);
    	CheckGL();

	shader.LoadShineVariables(1, 0);
	CheckGL();
}

void TerrainRenderer::BindTextures(Terrain &terrain)
{
	TerrainTexturePack& texturePack = terrain.texturePack;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturePack.getBackgroundTexture().getTextureId());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texturePack.getrTexture().getTextureId());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texturePack.getgTexture().getTextureId());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texturePack.getbTexture().getTextureId());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, terrain.blendMap.getTextureId());
    CheckGL();
}

void TerrainRenderer::UnbindTexturedModel()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
    CheckGL();
}

void TerrainRenderer::LoadModelMatrix(Terrain& terrain)
{
	Mat4 transformationMatrix = Mat4::TRS(terrain.position, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0, 1.0});
	shader.LoadTransformationMatrix(transformationMatrix);
}

