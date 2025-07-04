#include "AudioComponent.hpp"
#include "Logger.hpp"
#include <fmod_errors.h>
#include "AudioSystem.hpp"
#include "GameObject.hpp"

ELIX_NAMESPACE_BEGIN

void AudioComponent::setAudio(const std::string& soundPath)
{
    auto system = audio::AudioSystem::instance().getSystem();

    if (!system) 
    {
        ELIX_LOG_ERROR("AudioSystem not initialized — can't set sound");
        return;
    }

    if (m_sound)
    {
        m_sound->release();
        m_sound = nullptr;
    }

    m_path = soundPath;

    auto result = system->createSound(m_path.c_str(), FMOD_3D, nullptr, &m_sound);

    if (result != FMOD_OK) 
    {
        ELIX_LOG_ERROR("Failed to load sound '", m_path, "': ", FMOD_ErrorString(result));
        m_sound = nullptr;
        return;
    }

    ELIX_LOG_INFO("Sound attached successfully");
}

void AudioComponent::play() 
{
    if (!m_sound)
        return;

    auto system = audio::AudioSystem::instance().getSystem();

    if (!system)
        return;

    auto result = system->playSound(m_sound, nullptr, true, &m_channel);
    
    if (result != FMOD_OK)
    {
        ELIX_LOG_ERROR("Failed to play sound '", m_path, "': ", FMOD_ErrorString(result));
        m_channel = nullptr;
        return;
    }

    setVolume(m_volume);
    setLooping(m_looping);

    if (getOwner())
        setPosition(getOwner()->getPosition());

    m_channel->setPaused(false);
}

void AudioComponent::pause() 
{
    if (m_channel) 
        m_channel->setPaused(true);
}

void AudioComponent::stop()
{
    if (m_channel)
        m_channel->stop();
}

bool AudioComponent::isPlaying() const
{
    bool playing = false;

    if (m_channel) 
        m_channel->isPlaying(&playing);

    return playing;
}

void AudioComponent::setLooping(bool value) 
{
    m_looping = value;
    if (m_sound)
        m_sound->setMode(value ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
}

bool AudioComponent::isLooping() const 
{
    return m_looping;
}

void AudioComponent::setVolume(float volume) 
{
    m_volume = volume;

    if (m_channel)
        m_channel->setVolume(volume);
}

float AudioComponent::getVolume() const 
{
    float volume = 0.0f;

    if (m_channel)
        m_channel->getVolume(&volume);

    return volume;
}

void AudioComponent::setPosition(const glm::vec3& position) 
{
    if (m_channel) 
    {
        FMOD_VECTOR fmodPos = { position.x, position.y, position.z };
        FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
        m_channel->set3DAttributes(&fmodPos, &vel);
    }
}

void AudioComponent::update(float)
{
    if (getOwner())
        setPosition(getOwner()->getPosition());
}

void AudioComponent::destroy()
{
    Component::destroy();

    if (m_sound) 
    {
        m_sound->release();
        m_sound = nullptr;
    }
}

const std::string& AudioComponent::getSoundPath() const 
{
    return m_path;
}
ELIX_NAMESPACE_END
