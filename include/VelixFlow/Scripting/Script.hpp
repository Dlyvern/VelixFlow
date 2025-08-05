#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <string>
#include "VelixFlow/GameObject.hpp"

ELIX_NAMESPACE_BEGIN

class GameObject;

namespace scripting
{
    class Script
    {
    public:
        virtual ~Script() = default;

        virtual void onStart() = 0;
        virtual void onUpdate(float deltaTime) = 0;
        virtual std::string getScriptName() const = 0;

        void setOwner(GameObject* owner) { owner_ = owner; }

    protected:
        GameObject* owner_{nullptr};
    };
}

ELIX_NAMESPACE_END

#endif //SCRIPT_HPP
