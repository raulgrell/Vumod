#pragma once

#include <particles/ParticleTexture.h>

#include <math/Math.h>
#include <camera/Camera.h>

struct Particle
{
    Vec3 position;
    Vec3 velocity;
    float gravityEffect;
    float lifeLength;
    float rotation;
    float scale;
    float elapsedTime = 0;
    float distance = 0;

    ParticleTexture *texture;

    Vec2 texOffset1;
    Vec2 texOffset2;
    float blend {};

public:
    Particle(ParticleTexture *texture, Vec3 &position, Vec3 &velocity, float gravityEffect,
             float lifeLength, float rotation, float scale);

    // Particles with transparency must be drawn back to front. std::sort
    bool operator<(const Particle &r) const;

    bool Update(Camera &camera);

private:

    void UpdateTextureCoordInfo();
    void SetTextureOffset(Vec2 &offset, int index);
};

