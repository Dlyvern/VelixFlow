#ifndef ICOMPONENT_HPP
#define ICOMPONENT_HPP

#include "VelixFlow/DefaultMacros.hpp"

ELIX_NAMESPACE_BEGIN

class GameObject;

namespace components
{
    class IComponent
    {
    public:
        virtual void update(float deltaTime) {}
        virtual void destroy() {}

        virtual void setOwner(GameObject* owner) { m_owner = owner; }
        [[nodiscard]] GameObject* getOwner() const { return m_owner; }

        virtual ~IComponent() = default;
    private:
        GameObject* m_owner{nullptr};
    };
} //namespace component

ELIX_NAMESPACE_END

#endif //ICOMPONENT_HPP
