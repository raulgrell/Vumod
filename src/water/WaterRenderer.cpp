#include "WaterRenderer.h"

#include <glad/glad.h>
#include <scene/SceneRenderer.h>
#include <input/InputManager.h>
#include <platform/DisplayManager.h>

WaterRenderer::WaterRenderer(
	Loader& loader,
	WaterShader& shader,
	Mat4& projectionMatrix,
	float nearPlane,
	float farPlane,
	WaterFrameBuffers& fbos) : shader(shader), fbos(fbos)
{
	dudvTexture = loader.LoadTexture(DUDV_MAP);
	normalMap = loader.LoadTexture(NORMAL_MAP);
    shader.Bind();
	shader.connectTextureUnits();
	shader.LoadProjectionMatrix(projectionMatrix);
	shader.LoadNearPlane(nearPlane);
	shader.LoadFarPlane(farPlane);
	shader.LoadSkyColor(SceneRenderer::skyColour);
	shader.LoadFogVariables(SceneRenderer::fogDensity, SceneRenderer::fogGradient);
    shader.Unbind();
	setUpVAO(loader);
}

void WaterRenderer::render(std::vector<WaterTile*>& water, Camera& camera, Light& sun)
{
	prepareRender(camera, sun);
	for (WaterTile* tile : water) {
		Vec3 position(tile->getX(), tile->getHeight(), tile->getZ());
		Mat4 matrix = Mat4::TRS(position, {0.0f, 0.0f, 0.0f}, tile->getScale());
		shader.LoadTransformationMatrix(matrix);
		glDrawArrays(GL_TRIANGLES, 0, quad.getVertexCount());
	}
	unbind();
}

void WaterRenderer::prepareRender(Camera& camera, Light& sun)
{
    shader.Bind();
	shader.LoadViewMatrix(camera);

	shader.LoadWaterTiling(waterTiling);
	moveFactor += waveSpeed * DisplayManager::getFrameTimeSeconds();
	moveFactor = fmod(moveFactor, 1.0);
	shader.LoadMoveFactor(moveFactor);
	shader.LoadWaveStrength(waveStrength);
	shader.LoadWaterReflectivity(waterReflectivity);
	shader.LoadLight(sun);
	shader.LoadShineVariables(shineDamper, reflectivity);
	glBindVertexArray(quad.getVaoID());
	glEnableVertexAttribArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbos.getReflectionTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fbos.getRefractionTexture());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dudvTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normalMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, fbos.getRefractionDepthTexture());
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void WaterRenderer::unbind()
{
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
    shader.Unbind();
}

void WaterRenderer::setUpVAO(Loader& loader)
{
	// Just x and z vertex positions, y is set in shader
	std::vector<float> vertices = { -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, 0.5, 0.5 };
	quad = loader.LoadToVAO(vertices, 2);
}
