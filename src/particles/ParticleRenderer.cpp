#include "ParticleRenderer.h"

ParticleRenderer::ParticleRenderer(Mat4 &projectionMatrix)
{
    buffer.resize(INSTANCE_DATA_LENGTH * MAX_INSTANCES);
    vboId = Loader::CreateEmptyVbo(buffer);

    std::vector<float> vertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f};
    auto vao = std::make_shared<VertexArray>();

    auto pos = std::make_shared<VertexBuffer>(&vertices[0], vertices.size() * 1 * 4);
    pos->SetLayout({{0, GL_FLOAT, 3}});

    auto inst = std::make_shared<VertexBuffer>(&vertices[0], vertices.size() * 21 * 4);
    inst->SetLayout({{0, GL_FLOAT, 4},
                     {0, GL_FLOAT, 4},
                     {0, GL_FLOAT, 4},
                     {0, GL_FLOAT, 4},
                     {0, GL_FLOAT, 4},
                     {0, GL_FLOAT, 1}});

    vao->AddVertexBuffer(pos);
    vao->Unbind();

    quad = std::make_shared<RawModel>(vao, 4);
    // modelViewMatrix
    Loader::AddInstancedAttribute(quad->GetVaoId(), vboId, 1, 4, INSTANCE_DATA_LENGTH, 0);
    Loader::AddInstancedAttribute(quad->GetVaoId(), vboId, 2, 4, INSTANCE_DATA_LENGTH, 4);
    Loader::AddInstancedAttribute(quad->GetVaoId(), vboId, 3, 4, INSTANCE_DATA_LENGTH, 8);
    Loader::AddInstancedAttribute(quad->GetVaoId(), vboId, 4, 4, INSTANCE_DATA_LENGTH, 12);
    // texOffsets
    Loader::AddInstancedAttribute(quad->GetVaoId(), vboId, 5, 4, INSTANCE_DATA_LENGTH, 16);
    // blendFactor
    Loader::AddInstancedAttribute(quad->GetVaoId(), vboId, 6, 1, INSTANCE_DATA_LENGTH, 20);
    shader.Bind();
    shader.LoadProjectionMatrix(&projectionMatrix);
    shader.Unbind();
}

void
ParticleRenderer::Render(std::unordered_map<ParticleTexture *, std::vector<Particle>> &particlesMap, Camera &camera)
{
    Mat4 viewMatrix = camera.GetViewMatrix();

    Prepare();

    for (auto &mit : particlesMap) {
        ParticleTexture *texture = mit.first;
        std::vector<Particle> &particles = mit.second;
        BindTexture(texture);
        cursor = 0;
        std::vector<float> vboData(particles.size() * INSTANCE_DATA_LENGTH);

        for (auto &particle : particles) {
            UpdateModelViewMatrix(particle.position, particle.rotation, particle.scale, viewMatrix, vboData);
            UpdateTexCoordInfo(particle, vboData);
        }
        Loader::UpdateVbo(vboId, vboData);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad->GetVertexCount(), particles.size());

    }

    FinishRendering();
}

void ParticleRenderer::UpdateTexCoordInfo(Particle &particle, std::vector<float> &vboData)
{
    vboData[cursor++] = particle.texOffset1.x;
    vboData[cursor++] = particle.texOffset1.y;
    vboData[cursor++] = particle.texOffset2.x;
    vboData[cursor++] = particle.texOffset2.y;
    vboData[cursor++] = particle.blend;
}

void ParticleRenderer::BindTexture(ParticleTexture *texture)
{
    if (texture->IsAdditive()) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    } else {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureId());
    shader.LoadNumberOfRows(texture->GetNumberOfRows());
}

void ParticleRenderer::UpdateModelViewMatrix(
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
    StoreMatrixData(modelViewMatrix, vboData);
}

void ParticleRenderer::StoreMatrixData(Mat4 &matrix, std::vector<float> &vboData)
{
    vboData[cursor++] = matrix.r[0].c[0];
    vboData[cursor++] = matrix.r[1].c[0];
    vboData[cursor++] = matrix.r[2].c[0];
    vboData[cursor++] = matrix.r[3].c[0];
    vboData[cursor++] = matrix.r[0].c[1];
    vboData[cursor++] = matrix.r[1].c[1];
    vboData[cursor++] = matrix.r[2].c[1];
    vboData[cursor++] = matrix.r[3].c[1];
    vboData[cursor++] = matrix.r[0].c[2];
    vboData[cursor++] = matrix.r[1].c[2];
    vboData[cursor++] = matrix.r[2].c[2];
    vboData[cursor++] = matrix.r[3].c[2];
    vboData[cursor++] = matrix.r[0].c[3];
    vboData[cursor++] = matrix.r[1].c[3];
    vboData[cursor++] = matrix.r[2].c[3];
    vboData[cursor++] = matrix.r[3].c[3];
}

void ParticleRenderer::Prepare()
{
    shader.Bind();
    glBindVertexArray(quad->GetVaoId());
    for (GLuint i = 0; i <= 6; i++) {
        glEnableVertexAttribArray(i);
    }
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);

}

void ParticleRenderer::FinishRendering()
{
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    for (GLuint i = 0; i <= 6; i++) {
        glDisableVertexAttribArray(i);
    }
    glBindVertexArray(0);
    shader.Unbind();
}
