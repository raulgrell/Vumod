#pragma once

#include <graphics/Loader.h>
#include "ParticleShader.h"
#include "Particle.h"
#include "ParticleTexture.h"

#include <unordered_map>

class ParticleRenderer
{
public:
    ParticleRenderer(Loader &loader, Mat4 &projectionMatrix);
    void render(std::unordered_map<ParticleTexture *, std::vector<Particle>> &particlesMap, Camera &camera);
    void updateTexCoordInfo(Particle &particle, std::vector<GLfloat> &vboData);
    void bindTexture(ParticleTexture *texture);
    void updateModelViewMatrix(Vec3 &position, GLfloat rotation, GLfloat scale, Mat4 &viewMatrix,
                               std::vector<GLfloat> &vboData);
    void storeMatrixData(Mat4 &matrix, std::vector<GLfloat> &vboData);
    void prepare();
    void finishRendering();

    void bindTextures(Particle &particle);
    void LoadModelMatrix(Particle &particle);
    void unbindTexturedModel();

private:
    static constexpr int MAX_INSTANCES = 10000;
    static constexpr int INSTANCE_DATA_LENGTH = 21; // 16 (model view matrix) + 4 (texture info) + 1 (blend factor)
    RawModel quad;
    ParticleShader shader;
    Loader &loader;
    GLuint vboID;
    std::vector<GLfloat> buffer;
    int pointer {}; // where in the buffer we are writing
};
