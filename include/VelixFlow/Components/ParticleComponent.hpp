#ifndef PARTICLE_COMPONENT_HPP
#define PARTICLE_COMPONENT_HPP

#include "VelixFlow/ParticleSystem.hpp"
#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/Components/Component.hpp"

ELIX_NAMESPACE_BEGIN

namespace components
{
    class ParticleComponent : public IComponent
    {
    public:
        ParticleComponent(std::unique_ptr<ParticleSystem> system);
        void update(float deltaTime) override;
        void render();
    private:
        std::unique_ptr<ParticleSystem> m_particleSystem{nullptr};
    };
    
} //namespace components

ELIX_NAMESPACE_END

#endif //PARTICLE_COMPONENT_HPP
