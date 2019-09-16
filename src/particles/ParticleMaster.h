#pragma once

#include <graphics/Loader.h>

#include "Particle.h"
#include "ParticleRenderer.h"
#include "ParticleTexture.h"

class ParticleMaster
{
public:
    ParticleMaster(Loader &loader, Mat4 &projectionMatrix);
    static void update(Camera &camera);
    void renderParticles(Camera &camera);
    static void addParticle(Particle particle);
private:
    ParticleRenderer renderer;

    inline static std::unordered_map<ParticleTexture *, std::vector<Particle>> particlesMap;
};
