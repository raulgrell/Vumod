#pragma once

#include <particles/ParticleTexture.h>

#include <glad/glad.h>
#include <math/Math.h>
#include <scene/Camera.h>

struct Particle
{
    Vec3 position;
    Vec3 velocity;
    GLfloat gravityEffect; // <= 1.0
    GLfloat lifeLength;
    GLfloat rotation;
    GLfloat scale;
    GLfloat elapsedTime = 0.0f;
    GLfloat distance {};

    ParticleTexture *texture;

    Vec2 texOffset1;
    Vec2 texOffset2;
    GLfloat blend {};

public:
    Particle(ParticleTexture *texture, Vec3 &position, Vec3 &velocity, GLfloat gravityEffect,
             GLfloat lifeLength, GLfloat rotation, GLfloat scale);

    // Particles with transparency must be drawn back to front. std::sort
    bool operator<(const Particle &r) const;

    bool update(Camera &camera);

private:

    void updateTextureCoordInfo();
    void setTextureOffset(Vec2 &offset, int index);
};

