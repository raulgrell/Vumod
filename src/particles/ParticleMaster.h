#pragma once

#include <graphics/Loader.h>

#include "Particle.h"
#include "ParticleRenderer.h"
#include "ParticleTexture.h"

class ParticleMaster
{
    ParticleRenderer renderer;

public:
    ParticleMaster(Loader &loader, Mat4 &projectionMatrix);

    void Update(Camera &camera);
    void Render(Camera &camera);

    static void AddParticle(Particle particle);

private:
    inline static std::unordered_map<ParticleTexture *, std::vector<Particle>> particlesMap;
};
