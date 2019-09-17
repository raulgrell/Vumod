#include "TerrainRenderer.h"

#include <glad/glad.h>
#include <scene/SceneShader.h>

TerrainRenderer::TerrainRenderer(TerrainShader& shader, Mat4& projectionMatrix) : shader(shader)
{
    shader.Bind();
	shader.LoadProjectionMatrix(projectionMatrix);
	CHECK_GL();
	shader.connectTextureUnits();
    shader.Unbind();
}

void TerrainRenderer::Render(std::vector<Terrain *> *terrains)
{
	for (auto terrain : *terrains) {
        Prepare(*terrain);
		LoadModelMatrix(*terrain);
	CHECK_GL();
		glDrawElements(GL_TRIANGLES, terrain->model.vertexCount, GL_UNSIGNED_INT, nullptr);
	CHECK_GL();
       UnbindTexturedModel();
	CHECK_GL();
	}
}

void TerrainRenderer::Prepare(Terrain& terrain)
{
	glBindVertexArray(terrain.model.vaoId);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	CHECK_GL();

    BindTextures(terrain);
    	CHECK_GL();

	shader.LoadShineVariables(1, 0);
	CHECK_GL();
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
    CHECK_GL();
}

void TerrainRenderer::UnbindTexturedModel()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
    CHECK_GL();
}

void TerrainRenderer::LoadModelMatrix(Terrain& terrain)
{
	Mat4 transformationMatrix = Mat4::TRS(terrain.position, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0, 1.0});
	shader.LoadTransformationMatrix(transformationMatrix);
}

