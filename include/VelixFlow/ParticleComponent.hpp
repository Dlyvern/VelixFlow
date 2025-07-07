#ifndef PARTICLE_COMPONENT_HPP
#define PARTICLE_COMPONENT_HPP

#include "Component.hpp"
#include "ParticleSystem.hpp"

class ParticleComponent : public Component
{
public:
    ParticleComponent(std::unique_ptr<elix::ParticleSystem> system);
    void update(float deltaTime) override;
    void render();
private:
    std::unique_ptr<elix::ParticleSystem> m_particleSystem{nullptr};
};

#endif //PARTICLE_COMPONENT_HPP
