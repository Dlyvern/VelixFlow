#ifndef ANIMATOR_COMPONENT_HPP
#define ANIMATOR_COMPONENT_HPP

#include "VelixFlow/Components/Component.hpp"
#include "VelixFlow/Animation.hpp"
#include "VelixFlow/Skeleton.hpp"
#include "VelixFlow/DefaultMacros.hpp"

ELIX_NAMESPACE_BEGIN

namespace components
{
    class AnimatorComponent final : public IComponent
    {
    public:
        void update(float deltaTime) override;

        void playAnimation(animation::Animation* animation, bool repeat = true);

        void stopAnimation();

        [[nodiscard]] bool isAnimationPlaying() const;
    private:
        void calculateBoneTransform(Skeleton::BoneInfo* boneInfo, const glm::mat4 &parentTransform, animation::Animation* animation, float currentTime);
        void calculateObjectTransform(animation::Animation* animation, float currentTime);

        bool m_isAnimationPaused{false};
        bool m_isAnimationLooped{true};
        bool m_isAnimationCompleted{false};
        bool m_isInterpolating{false};

        float m_animationSpeed{1.0f};
        float m_currentTime{0.0f};
        float m_haltTime{0.0f};
        float m_interTime{0.0f};

        animation::Animation* m_currentAnimation{nullptr};
        animation::Animation* m_nextAnimation{nullptr};
        animation::Animation* m_queueAnimation{nullptr};
    };
}

ELIX_NAMESPACE_END

#endif //ANIMATOR_COMPONENT_HPP
