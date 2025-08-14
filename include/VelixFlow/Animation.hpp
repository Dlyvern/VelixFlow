#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "VelixFlow/DefaultMacros.hpp"

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>
#include <string>
#include <algorithm>

ELIX_NAMESPACE_BEGIN

class Skeleton;
class GameObject;

namespace animation
{
    struct SQT
    {
        glm::quat rotation{1, 0, 0, 0};
        glm::vec3 position{0, 0, 0};
        glm::vec3 scale{0, 0, 0};
        float timeStamp{0.0f};
    };

    struct AnimationTrack
    {
        std::vector<SQT> keyFrames;
        std::string objectName;
    };

    struct Animation
    {
        std::string name;
        double ticksPerSecond;
        double duration;
        std::vector<AnimationTrack> boneAnimations;
        Skeleton* skeletonForAnimation{nullptr};
        GameObject* gameObject{nullptr};

        AnimationTrack* getAnimationTrack(const std::string& name)
        {
            const auto it = std::find_if(boneAnimations.begin(), boneAnimations.end(), [&name](const auto& bone) {return bone.objectName == name;});
            return it == boneAnimations.end() ? nullptr : &(*it);
        }
    };
} //namespace animation

ELIX_NAMESPACE_END

#endif //ANIMATION_HPP