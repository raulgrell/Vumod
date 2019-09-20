#include <platform/DisplayManager.h>
#include "Particle.h"
#include "ParticleMaster.h"

Particle::Particle(ParticleTexture *texture,
                   Vec3 &position,
                   Vec3 &velocity,
                   GLfloat gravityEffect,
                   GLfloat lifeLength,
                   GLfloat rotation,
                   GLfloat scale)
{
    this->texture = texture;
    this->position = position;
    this->velocity = velocity;
    this->gravityEffect = gravityEffect;
    this->lifeLength = lifeLength;
    this->rotation = rotation;
    this->scale = scale;
    ParticleMaster::AddParticle(*this);
}

bool Particle::operator<(const Particle &r) const
{
    return this->distance > r.distance;
}

bool Particle::update(Camera &camera)
{
    velocity.y += gravityEffect * DisplayManager::getFrameTimeSeconds();
    Vec3 change(velocity);
    change *= DisplayManager::getFrameTimeSeconds();
    position += change;
    Vec3 d = camera.GetPosition() - position;
    distance = d.x * d.x + d.y * d.y + d.z * d.z;
    updateTextureCoordInfo();
    elapsedTime += DisplayManager::getFrameTimeSeconds();
    return elapsedTime < lifeLength;
}

void Particle::updateTextureCoordInfo()
{
    GLfloat lifeFactor = elapsedTime / lifeLength;
    int stageCount = texture->GetNumberOfRows() * texture->GetNumberOfRows();
    GLfloat atlasProgression = lifeFactor * stageCount;
    int index1 = (int) floor(atlasProgression);
    int index2 = index1 < stageCount - 1 ? index1 + 1 : index1;
    this->blend = atlasProgression - (GLfloat) index1;

    setTextureOffset(texOffset1, index1);
    setTextureOffset(texOffset2, index2);
}

void Particle::setTextureOffset(Vec2 &offset, int index)
{
    int column = index % texture->GetNumberOfRows();
    int row = index / texture->GetNumberOfRows();
    offset.x = (GLfloat) column / texture->GetNumberOfRows();
    offset.y = (GLfloat) row / texture->GetNumberOfRows();
}
