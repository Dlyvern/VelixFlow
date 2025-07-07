#ifndef AUIDO_COMPONENT_HPP
#define AUIDO_COMPONENT_HPP

#include "DefaultMacros.hpp"
#include "Component.hpp"
#include "fmod.hpp"
#include <glm/vec3.hpp>
#include <string>

ELIX_NAMESPACE_BEGIN

class AudioComponent : public Component
{
public:
    void setAudio(const std::string& path);

    void play();
    void pause();
    void stop();
    bool isPlaying() const;

    void setLooping(bool value);
    void setVolume(float volume);
    float getVolume() const;

    bool isLooping() const;

    void setPosition(const glm::vec3& position);
    void update(float deltaTime) override;
    void destroy() override;

    const std::string& getSoundPath() const;
private:
    std::string m_path;
    FMOD::Sound* m_sound = nullptr;
    FMOD::Channel* m_channel = nullptr;
    bool m_looping = false;
    float m_volume = 1.0f;
};

ELIX_NAMESPACE_END

#endif //AUIDO_COMPONENT_HPP