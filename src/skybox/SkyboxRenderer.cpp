
#include "SkyboxRenderer.h"
#include "math/Math.h"

#include <glad/glad.h>
#include <platform/DisplayManager.h>
#include <scene/SceneShader.h>
#include "scene/Camera.h"


SkyboxRenderer::SkyboxRenderer(Loader& loader, Mat4& projectionMatrix)
{
	cube = loader.LoadToVAO(VERTICES, 3);
	texture = loader.LoadCubeMap(TEXTURE_FILES);
	nightTexture = loader.LoadCubeMap(NIGHT_TEXTURE_FILES);
    shader.Bind();
    CheckGL();
    shader.ConnectTextureUnits();
    CheckGL();
    shader.LoadProjectionMatrix(&projectionMatrix);
    CheckGL();
    shader.Unbind();
    CheckGL();
}

void SkyboxRenderer::Render(Camera *camera, float r, float g, float b)
{
    shader.Bind();
	shader.LoadViewMatrix(camera);
	shader.LoadFogColor(r, g, b);
    CheckGL();

	// Disable this, otherwise one side of the sky is clipped from the
	// reflection and refraction textures
	// glDisable(GL_CLIP_DISTANCE0);
	glBindVertexArray(cube.vaoID);
	glEnableVertexAttribArray(0);
    CheckGL();

    BindTextures();
	glDrawArrays(GL_TRIANGLES, 0, cube.vertexCount);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
    shader.Unbind();
}

void SkyboxRenderer::BindTextures()
{
	myTime += DisplayManager::getFrameTimeSeconds() * 100;
	while (myTime >= 24000) {
		myTime -= 24000;
	}
	int texture1, texture2;
	float blendFactor;
	if (myTime < 5000) {
		texture1 = nightTexture;
		texture2 = nightTexture;
		blendFactor = (myTime - 0) / (5000 - 0);
	}
	else if (myTime >= 5000 && myTime < 8000) {
		texture1 = nightTexture;
		texture2 = texture;
		blendFactor = (myTime - 5000) / (8000 - 5000);
	}
	else if (myTime >= 8000 && myTime < 21000) {
		texture1 = texture;
		texture2 = texture;
		blendFactor = (myTime - 8000) / (21000 - 8000);
	}
	else {
		texture1 = texture;
		texture2 = nightTexture;
		blendFactor = (myTime - 21000) / (24000 - 21000);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture2);
	shader.LoadBlendFactor(blendFactor);
    CheckGL();
}
