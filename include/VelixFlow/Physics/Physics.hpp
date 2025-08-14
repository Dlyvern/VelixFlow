#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <memory>

#if VELIX_USE_PHYSX
    #include "PxConfig.h"
    #include "PxPhysicsAPI.h"
#endif

#include "VelixFlow/GameObject.hpp"

//TODO Make clear PxScene in different Scenes
namespace physics
{
    class PhysicsController
    {
    public:
        void init();
        static PhysicsController& instance();
        void simulate(float deltaTime);
        void resizeCollider(const glm::vec3& newSize, std::shared_ptr<elix::GameObject> collider);

        void release();

    #if VELIX_USE_PHYSX
        [[nodiscard]] physx::PxRigidDynamic* addDynamicActor(std::shared_ptr<elix::GameObject> actor) const;
        [[nodiscard]] physx::PxRigidStatic* addStaticActor(std::shared_ptr<elix::GameObject> actor) const;
        [[nodiscard]] physx::PxControllerManager* getControllerManager() const;
        [[nodiscard]] physx::PxMaterial* getDefaultMaterial() const;
        [[nodiscard]] physx::PxScene* getScene() const;
    #endif
    private:
    #if VELIX_USE_PHYSX
        physx::PxPhysics* m_physics{nullptr};
        physx::PxScene* m_scene{nullptr};
        physx::PxFoundation* m_foundation{nullptr};
        physx::PxDefaultAllocator m_defaultAllocator;
        physx::PxPvd* m_pvd{nullptr};
        physx::PxControllerManager* m_controllerManager{nullptr};
        physx::PxMaterial* m_defaultMaterial{nullptr};
    #endif
    };
} //namespace physics

#endif //PHYSICS_HPP
