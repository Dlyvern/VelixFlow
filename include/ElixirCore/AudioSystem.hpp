#ifndef AUDIO_SYSTEM_HPP
#define AUDIO_SYSTEM_HPP

#include "DefaultMacros.hpp"
#include <fmod.hpp>

ELIX_NAMESPACE_BEGIN

namespace audio
{
    class AudioSystem
    {
    public:
        static AudioSystem& instance();

        FMOD::System* getSystem();
        void init();
        void update();
        void shutdown();
    private:
        FMOD::System* m_system{nullptr};
    };
}

ELIX_NAMESPACE_END


#endif //AUDIO_SYSTEM_HPP
