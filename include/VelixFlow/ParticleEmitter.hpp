#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include <memory>
#include <vector>

#include <glm/vec3.hpp>

namespace elix
{
    struct Particle
    {
        glm::vec3 position;
        glm::vec3 velocity;
        float lifeTime;
    };

    class ParticleModule
    {
    public:
        virtual void update(Particle& particle, float deltaTime) = 0;
        virtual void onSpawn(Particle& particle) = 0;
        virtual void onDeath(Particle& particle) = 0;
        virtual ~ParticleModule() = default;
    };

    class ParticleEmitter
    {
    public:
        void update(float deltaTime);
        void render();
        
        void addModule(std::shared_ptr<ParticleModule> particleModule);

        void setInitialVelocity(glm::vec3 velocity) { m_initialVelocity = velocity; }
        void setLifetime(float life) { m_lifetime = life; }
        void setSpawnRate(float rate) { m_spawnRate = rate; }
    private:
        std::vector<std::shared_ptr<ParticleModule>> m_modules;
        std::vector<Particle> m_particles;
        float m_spawnRate = 100.0f;
        float m_spawnTimer = 0.0f;
        float m_lifetime = 2.0f;
        glm::vec3 m_initialVelocity = glm::vec3(0.0f);
    };

} //namespace elix
#endif //PARTICLE_EMITTER_HPP
