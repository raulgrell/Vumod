#include <util/Utils.h>
#include <platform/DisplayManager.h>
#include "ParticleSystem.h"
#include "Particle.h"
#include "ParticleTexture.h"

ParticleSystem::ParticleSystem(ParticleTexture &texture,
                               float pps,
                               float speed,
                               float gravityCompliant,
                               float lifeLength,
                               float scale) : texture(texture)
{
    this->pps = pps;
    this->averageSpeed = speed;
    this->gravityCompliant = gravityCompliant;
    this->averageLifeLength = lifeLength;
    this->averageScale = scale;
}

void ParticleSystem::SetDirection(Vec3 direction, float deviation)
{
    this->direction = direction;
    this->haveDirection = true;
    this->directionDeviation = deviation;
}

void ParticleSystem::RandomizeRotation()
{
    randomRotation = true;
}

void ParticleSystem::GenerateParticles(Vec3 systemCenter)
{
    float delta = DisplayManager::getFrameTimeSeconds();
    float particlesToCreate = pps * delta;
    int count = floor(particlesToCreate);
    float partialParticle = particlesToCreate - (float) count;
    for (int i = 0; i < count; i++) {
        EmitParticle(systemCenter);
    }
    if (Random::Float() < partialParticle) {
        EmitParticle(systemCenter);
    }
}

void ParticleSystem::EmitParticle(Vec3 &center)
{
    //float dirX = Random::Float()) * 2.0 - 1.0;
    //float dirZ = Random::Float()) * 2.0 - 1.0;
    //Vec3 velocity(dirX, 1, dirZ);
    Vec3 velocity;
    if (haveDirection) {
        velocity = GenerateRandomUnitVectorWithinCone(direction, directionDeviation);
    } else {
        velocity = GenerateRandomUnitVector();
    }
    velocity = Vec3::Normal(velocity);

    float speed = GenerateValue(averageSpeed, speedError);
    velocity *= speed;
    float scale = GenerateValue(averageScale, scaleError);
    float lifeLength = GenerateValue(averageLifeLength, lifeError);

    Particle particle(&texture, center, velocity, gravityCompliant, lifeLength, GenerateRotation(), scale);
}

float ParticleSystem::GenerateValue(float average, float errorMargin)
{
    float offset = (Random::Float() - 0.5) * 2 * errorMargin;
    return average + offset;
}

float ParticleSystem::GenerateRotation()
{
    if (randomRotation) {
        return Random::Float() * 360;
    } else {
        return 0;
    }
}

Vec3 ParticleSystem::GenerateRandomUnitVectorWithinCone(Vec3 &coneDirection, float angle)
{
    float cosAngle = cos(angle);
    float theta = Random::Float() * 2 * M_PI;
    float z = cosAngle + Random::Float() * (1 - cosAngle);
    float rootOneMinusZSquared = sqrt(1 - z * z);
    float x = rootOneMinusZSquared * cos(theta);
    float y = rootOneMinusZSquared * sin(theta);

    Vec4 direction(x, y, z, 1);

    if (coneDirection.x != 0 || coneDirection.y != 0 || (coneDirection.z != 1 && coneDirection.z != -1)) {
        Vec3 k(0, 0, 1);
        Vec3 rotateAxis = Vec3::Cross(coneDirection, k);
        rotateAxis = Vec3::Normal(rotateAxis);
        float value = Vec3::Dot(coneDirection, k);
        float rotateAngle = acos(value);
        Mat4 rotationMatrix = Mat4::Rotate(-rotateAngle, rotateAxis);
        direction = rotationMatrix * direction;
    } else if (coneDirection.z == -1) {
        direction.z *= -1;
    }

    return Vec3(direction.x, direction.y, direction.z);
}

Vec3 ParticleSystem::GenerateRandomUnitVector()
{
    float theta = Random::Float() * 2 * M_PI;
    float z = Random::Float() * 2 - 1;
    float rootOneMinusZSquared = sqrt(1 - z * z);
    float x = rootOneMinusZSquared * cos(theta);
    float y = rootOneMinusZSquared * sin(theta);
    return Vec3(x, y, z);
}
