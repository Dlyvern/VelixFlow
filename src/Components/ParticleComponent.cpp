#include "VelixFlow/Components/ParticleComponent.hpp"

ELIX_NAMESPACE_BEGIN

namespace components
{
    ParticleComponent::ParticleComponent(std::unique_ptr<ParticleSystem> system)  
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
} //namespace components

ELIX_NAMESPACE_END