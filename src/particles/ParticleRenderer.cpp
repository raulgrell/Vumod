#include "ParticleRenderer.h"

ParticleRenderer::ParticleRenderer(Loader &loader, Mat4 &projectionMatrix)
        : loader(loader)
{
    buffer.resize(INSTANCE_DATA_LENGTH * MAX_INSTANCES);
    vboID = loader.createEmptyVbo(buffer);
    std::vector<GLfloat> vertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f};
    quad = loader.LoadToVAO(vertices, 2);
    // modelViewMatrix
    loader.addInstancedAttribute(quad.vaoID, vboID, 1, 4, INSTANCE_DATA_LENGTH, 0);
    loader.addInstancedAttribute(quad.vaoID, vboID, 2, 4, INSTANCE_DATA_LENGTH, 4);
    loader.addInstancedAttribute(quad.vaoID, vboID, 3, 4, INSTANCE_DATA_LENGTH, 8);
    loader.addInstancedAttribute(quad.vaoID, vboID, 4, 4, INSTANCE_DATA_LENGTH, 12);
    // texOffsets
    loader.addInstancedAttribute(quad.vaoID, vboID, 5, 4, INSTANCE_DATA_LENGTH, 16);
    // blendFactor
    loader.addInstancedAttribute(quad.vaoID, vboID, 6, 1, INSTANCE_DATA_LENGTH, 20);
    shader.Bind();
    shader.LoadProjectionMatrix(&projectionMatrix);
    shader.Unbind();
}


void
ParticleRenderer::render(std::unordered_map<ParticleTexture *, std::vector<Particle>> &particlesMap, Camera &camera)
{
    Mat4 viewMatrix = camera.GetViewMatrix();

    prepare();

    for (auto &mit : particlesMap) {
        ParticleTexture *texture = mit.first;
        std::vector<Particle> &particles = mit.second;
        bindTexture(texture);
        pointer = 0;
        std::vector<GLfloat> vboData(particles.size() * INSTANCE_DATA_LENGTH);

        for (auto &particle : particles) {
            updateModelViewMatrix(particle.position, particle.rotation, particle.scale, viewMatrix, vboData);
            updateTexCoordInfo(particle, vboData);
        }
        loader.updateVbo(vboID, vboData);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad.vertexCount, particles.size());

    }

    finishRendering();
}

void ParticleRenderer::updateTexCoordInfo(Particle &particle, std::vector<GLfloat> &vboData)
{
    vboData[pointer++] = particle.texOffset1.x;
    vboData[pointer++] = particle.texOffset1.y;
    vboData[pointer++] = particle.texOffset2.x;
    vboData[pointer++] = particle.texOffset2.y;
    vboData[pointer++] = particle.blend;
}

void ParticleRenderer::bindTexture(ParticleTexture *texture)
{
    if (texture->isAdditive()) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    } else {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
    shader.LoadNumberOfRows(texture->getNumberOfRows());
}

void ParticleRenderer::updateModelViewMatrix(
        Vec3 &position, float rotation, float scale, Mat4 &viewMatrix,
        std::vector<float> &vboData)
{
    Mat4 modelMatrix = Mat4::Translate(position);
    // Set the rotation 3x3 part of the model matrix to the
    // transpose of the 3x3 rotation part of the view matrix.
    modelMatrix.r[0].c[0] = viewMatrix.r[0].c[0];
    modelMatrix.r[1].c[0] = viewMatrix.r[0].c[1];
    modelMatrix.r[2].c[0] = viewMatrix.r[0].c[2];
    modelMatrix.r[0].c[1] = viewMatrix.r[1].c[0];
    modelMatrix.r[1].c[1] = viewMatrix.r[1].c[1];
    modelMatrix.r[2].c[1] = viewMatrix.r[1].c[2];
    modelMatrix.r[0].c[2] = viewMatrix.r[2].c[0];
    modelMatrix.r[1].c[2] = viewMatrix.r[2].c[1];
    modelMatrix.r[2].c[2] = viewMatrix.r[2].c[2];
    modelMatrix *= Mat4::Rotate(math::radians(rotation), Vec3(0.0f, 0.0f, 1.0f));
    modelMatrix *= Mat4::Scale(Vec3(scale));
    Mat4 modelViewMatrix = viewMatrix * modelMatrix;
    storeMatrixData(modelViewMatrix, vboData);
}

void ParticleRenderer::storeMatrixData(Mat4 &matrix, std::vector<GLfloat> &vboData)
{
    vboData[pointer++] = matrix.r[0].c[0];
    vboData[pointer++] = matrix.r[1].c[0];
    vboData[pointer++] = matrix.r[2].c[0];
    vboData[pointer++] = matrix.r[3].c[0];
    vboData[pointer++] = matrix.r[0].c[1];
    vboData[pointer++] = matrix.r[1].c[1];
    vboData[pointer++] = matrix.r[2].c[1];
    vboData[pointer++] = matrix.r[3].c[1];
    vboData[pointer++] = matrix.r[0].c[2];
    vboData[pointer++] = matrix.r[1].c[2];
    vboData[pointer++] = matrix.r[2].c[2];
    vboData[pointer++] = matrix.r[3].c[2];
    vboData[pointer++] = matrix.r[0].c[3];
    vboData[pointer++] = matrix.r[1].c[3];
    vboData[pointer++] = matrix.r[2].c[3];
    vboData[pointer++] = matrix.r[3].c[3];
}

void ParticleRenderer::prepare()
{
    shader.Bind();
    glBindVertexArray(quad.vaoID);
    for (GLuint i = 0; i <= 6; i++) {
        glEnableVertexAttribArray(i);
    }
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);

}

void ParticleRenderer::finishRendering()
{
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    for (GLuint i = 0; i <= 6; i++) {
        glDisableVertexAttribArray(i);
    }
    glBindVertexArray(0);
    shader.Unbind();
}
