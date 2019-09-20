#include <algorithm>
#include "ParticleMaster.h"

ParticleMaster::ParticleMaster(Mat4 &projectionMatrix)
        : renderer( projectionMatrix)
{
}

void ParticleMaster::Update(Camera &camera)
{
    auto mit = particlesMap.begin();
    while (mit != particlesMap.end()) {
        ParticleTexture *texture = mit->first;
        std::vector<Particle> &particles = mit->second;
        auto vit = particles.begin();
        while (vit != particles.end()) {
            Particle &p = *vit;
            bool stillAlive = p.Update(camera);
            if (!stillAlive) {
                vit = particles.erase(vit);
            } else {
                vit++;
            }
        }

        if (!texture->IsAdditive()) {
            // Alpha blended particles must be drawn back to front, uses operator <
            std::sort(particles.begin(), particles.end());
        }
        mit++;
    }
}

void ParticleMaster::Render(Camera &camera)
{
    renderer.Render(particlesMap, camera);
}

void ParticleMaster::AddParticle(Particle particle)
{
    ParticleTexture *texture = particle.texture;
    auto it = particlesMap.find(texture);
    if (it != particlesMap.end()) {
        std::vector<Particle> &particles = it->second;
        particles.push_back(particle);
    } else {
        std::vector<Particle> particles;
        particles.push_back(particle);
        particlesMap.insert(std::make_pair(texture, particles));
    }
}
