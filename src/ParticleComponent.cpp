#include "ParticleComponent.hpp"


ParticleComponent::ParticleComponent(std::unique_ptr<elix::ParticleSystem> system)  
{
    m_particleSystem = std::move(system);
}

void ParticleComponent::render()
{
    if(m_particleSystem)
        m_particleSystem->render();
}

void ParticleComponent::update(float deltaTime)
{
    if(m_particleSystem)
        m_particleSystem->update(deltaTime);
}
