#include "ParticleSystem.hpp"
#include "ParticleEmitter.hpp"


void elix::ParticleSystem::update(float deltaTime)
{
    for(auto& emitter : m_emitters)
        emitter->update(deltaTime);
}


void elix::ParticleSystem::render()
{
    for(auto& emitter : m_emitters)
        emitter->render();
}

void elix::ParticleSystem::addEmitter(std::unique_ptr<elix::ParticleEmitter> emitter)
{
    m_emitters.push_back(std::move(emitter));
}
