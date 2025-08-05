#include "VelixFlow/Components/RigidbodyComponent.hpp"
#include "VelixFlow/GameObject.hpp"
#include "VelixFlow/Components/TransformComponent.hpp"

#include <iostream>

ELIX_NAMESPACE_BEGIN

namespace components
{
    RigidbodyComponent::RigidbodyComponent(const std::shared_ptr<GameObject> &object)
    {
        m_rigidActor = physics::PhysicsController::instance().addStaticActor(object);

        if (!m_rigidActor)
            std::cerr << "RigidbodyComponent::RigidbodyComponent(): Failed to create physics body actor" << std::endl;

        object->getComponent<components::TransformComponent>()->positionChanged.connect(std::bind(&RigidbodyComponent::onOwnerPositionChanged, this, std::placeholders::_1));
    }

    void RigidbodyComponent::update(float deltaTime)
    {
        if (!m_rigidActor)
            return;

        if (auto owner = this->getOwner())
        {
            const physx::PxTransform transform = m_rigidActor->getGlobalPose();
            owner->getComponent<components::TransformComponent>()->setPosition({transform.p.x, transform.p.y, transform.p.z});
        }
    }

    physx::PxRigidActor* RigidbodyComponent::getRigidActor() const
    {
        return m_rigidActor;
    }

    void RigidbodyComponent::destroy()
    {
        IComponent::destroy();

        if (m_rigidActor)
        {
            m_rigidActor->release();
            m_rigidActor = nullptr;
        }
    }

    void RigidbodyComponent::onOwnerPositionChanged(const glm::vec3& position)
    {
        if (!m_rigidActor)
            return;
        m_rigidActor->setGlobalPose({position.x, position.y, position.z});
    }

} //namespace components

ELIX_NAMESPACE_END