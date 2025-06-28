#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include "ParticleEmitter.hpp"

namespace elix
{
    class ParticleSystem
    {
    public:
        void update(float deltaTime);
        void render();
        void addEmitter(std::unique_ptr<elix::ParticleEmitter> emitter);
    private:
        std::vector<std::unique_ptr<elix::ParticleEmitter>> m_emitters;
    };
} //namespace elix


#endif //PARTICLE_SYSTEM_HPP
