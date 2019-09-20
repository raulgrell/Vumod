#pragma once

#include "ParticleTexture.h"

#include <math/Math.h>

class ParticleSystem
{
public:
    ParticleSystem(ParticleTexture &texture,
                   float pps,
                   float speed,
                   float gravityCompliant,
                   float lifeLength,
                   float scale);

    void SetDirection(Vec3 direction, float deviation);
    void RandomizeRotation();

    void SetSpeedError(float error) { speedError = error * averageSpeed; }
    void SetLifeError(float error) { lifeError = error * averageLifeLength; }
    void SetScaleError(float error) { scaleError = error * averageScale; }

    void GenerateParticles(Vec3 systemCenter);
    void EmitParticle(Vec3 &center);
    float GenerateValue(float average, float errorMargin);
    float GenerateRotation();
    Vec3 GenerateRandomUnitVectorWithinCone(Vec3 &coneDirection, float angle);
    Vec3 GenerateRandomUnitVector();
private:
    ParticleTexture &texture;
    float pps;
    float averageSpeed;
    float gravityCompliant;
    float averageLifeLength;
    float averageScale;

    float speedError {};
    float lifeError {};
    float scaleError = 0;
    bool randomRotation = false;
    Vec3 direction;
    bool haveDirection = false;
    float directionDeviation = 0;
};

