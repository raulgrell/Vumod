#include "AnimatedModelRenderer.h"

void AnimatedModelRenderer::render(AnimatedModel entity, ICamera camera, Vector3f lightDir)
{
    prepare(camera, lightDir);
    entity.getTexture().bindToUnit(0);
    entity.getModel().bind(0, 1, 2, 3, 4);
    shader.jointTransforms.loadMatrixArray(entity.getJointTransforms());
    GL11.glDrawElements(GL11.GL_TRIANGLES, entity.getModel().getIndexCount(), GL11.GL_UNSIGNED_INT, 0);
    entity.getModel().unbind(0, 1, 2, 3, 4);
    finish();
}

void AnimatedModelRenderer::prepare(ICamera camera, Vector3f lightDir)
{
    shader.start();
    shader.projectionViewMatrix.loadMatrix(camera.getProjectionViewMatrix());
    shader.lightDirection.loadVec3(lightDir);
    OpenGlUtils.antialias(true);
    OpenGlUtils.disableBlending();
    OpenGlUtils.enableDepthTesting(true);
}
