#pragma once

#include <graphics/Loader.h>
#include "ParticleShader.h"
#include "Particle.h"
#include "ParticleTexture.h"

#include <unordered_map>

class ParticleRenderer
{
    static constexpr int MAX_INSTANCES = 10000;
    static constexpr int INSTANCE_DATA_LENGTH = 21; // 16 (model view matrix) + 4 (texture info) + 1 (blend factor)

public:
    ParticleRenderer(Mat4 &projectionMatrix);
    void Render(std::unordered_map<ParticleTexture *, std::vector<Particle>> &particlesMap, Camera &camera);
    void UpdateTexCoordInfo(Particle &particle, std::vector<GLfloat> &vboData);
    void BindTexture(ParticleTexture *texture);
    void UpdateModelViewMatrix(Vec3 &position, GLfloat rotation, GLfloat scale, Mat4 &viewMatrix,
                               std::vector<GLfloat> &vboData);
    void StoreMatrixData(Mat4 &matrix, std::vector<GLfloat> &vboData);
    void Prepare();
    void FinishRendering();

    void BindTextures(Particle &particle);
    void LoadModelMatrix(Particle &particle);
    void UnbindTexturedModel();

private:
    std::shared_ptr<IModel> quad;
    ParticleShader shader;
    unsigned int vboId;
    std::vector<float> buffer;
    int cursor = 0;
};
