#include "ParticleEmitter.hpp"
#include <glad/glad.h>

static GLuint particleVAO = 0;
static GLuint quadVBO = 0;
static GLuint instanceVBO = 0;
static bool buffersInitialized = false;


struct ParticleInstanceData {
    glm::vec3 position;
    glm::vec3 velocity;
};

static void initRenderBuffers() 
{
    float quadVertices[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f,  0.5f
    };

    glGenVertexArrays(1, &particleVAO);
    glBindVertexArray(particleVAO);

    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 10000, nullptr, GL_DYNAMIC_DRAW); // 10k max

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleInstanceData) * 10000, nullptr, GL_DYNAMIC_DRAW);

    // Instance attribute: position (layout = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleInstanceData), (void*)0);
    glVertexAttribDivisor(1, 1);

    // Instance attribute: velocity (layout = 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleInstanceData), (void*)(sizeof(glm::vec3)));
    glVertexAttribDivisor(2, 1);
}

void elix::ParticleEmitter::addModule(std::shared_ptr<elix::ParticleModule> particleModule)
{
    m_modules.push_back(particleModule);
}

void elix::ParticleEmitter::render()
{
    if (!buffersInitialized)
    {
        initRenderBuffers();
        buffersInitialized = true;
    }
    
    std::vector<ParticleInstanceData> instanceData;
    for (const auto& p : m_particles)
        instanceData.push_back({ p.position, p.velocity });

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, instanceData.size() * sizeof(ParticleInstanceData), instanceData.data());

    glBindVertexArray(particleVAO);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, static_cast<GLsizei>(instanceData.size()));
}

void elix::ParticleEmitter::update(float deltaTime)
{
    m_spawnTimer += deltaTime;
    const float spawnInterval = 1.0f / m_spawnRate;

    while (m_spawnTimer >= spawnInterval)
    {
        m_spawnTimer -= spawnInterval;
        Particle p;
        p.position = glm::vec3(0.0f);
        p.velocity = m_initialVelocity;
        p.lifeTime = m_lifetime;

        for (auto& mod : m_modules)
            mod->onSpawn(p);

        m_particles.push_back(p);
    }

    for (auto& particle : m_particles)
    {
        for (auto& module : m_modules)
            module->update(particle, deltaTime);
        particle.lifeTime -= deltaTime;
    }

    m_particles.erase(std::remove_if(m_particles.begin(), m_particles.end(),
    [](const auto& p) { return p.lifeTime <= 0.0f; }), m_particles.end());
}

