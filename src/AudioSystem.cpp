#include "AudioSystem.hpp"
#include "fmod.hpp"
#include "fmod_errors.h"
#include <iostream>

#define FMOD_CHECK(result) \
    if ((result) != FMOD_OK) { \
        std::cerr << "FMOD error: " << FMOD_ErrorString(result) << "\n"; \
    }

ELIX_NAMESPACE_BEGIN

audio::AudioSystem& audio::AudioSystem::instance()
{
    static audio::AudioSystem instance;
    return instance;
}

FMOD::System* audio::AudioSystem::getSystem()
{
    return m_system;
}

void audio::AudioSystem::update()
{
    if(m_system) m_system->update();
}

void audio::AudioSystem::init()
{
    auto result = FMOD::System_Create(&m_system);
    
    FMOD_CHECK(result);

    result = m_system->init(512, FMOD_INIT_3D_RIGHTHANDED, 0);

    FMOD_CHECK(result);
}

void audio::AudioSystem::shutdown()
{
    if(m_system)
    {
        m_system->close();
        m_system->release();
        m_system = nullptr;
    }
}

ELIX_NAMESPACE_END
