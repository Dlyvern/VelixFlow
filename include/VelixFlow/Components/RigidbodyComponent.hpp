#ifndef RIGID_BODY_COMPONENT_HPP
#define RIGID_BODY_COMPONENT_HPP

#include "VelixFlow/Physics/Physics.hpp"
#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/Components/Component.hpp"

#include <glm/vec3.hpp>
#include <memory>

ELIX_NAMESPACE_BEGIN

namespace components
{
    class RigidbodyComponent final : public IComponent
    {
    public:
        explicit RigidbodyComponent(const std::shared_ptr<GameObject>& object);

        void update(float deltaTime) override;

        [[nodiscard]] physx::PxRigidActor* getRigidActor() const;

        void destroy() override;
    private:
        physx::PxRigidActor* m_rigidActor{nullptr};

        void onOwnerPositionChanged(const glm::vec3& position);
    };
} //namespace components

ELIX_NAMESPACE_END

#endif //RIGID_BODY_COMPONENT_HPP
