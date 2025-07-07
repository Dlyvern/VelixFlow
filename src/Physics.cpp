#include "Physics.hpp"
#include <iostream>
#include "RigidbodyComponent.hpp"
#include "Logger.hpp"

struct UserErrorCallback final : physx::PxErrorCallback
{
     void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, const int line) override
     {
         std::cout << file << " line " << line << ": " << message << "\n" << "\n";
     }
} gErrorCallback;

void physics::PhysicsController::init()
{
#ifdef VELIX_USE_PHYSX
    m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_defaultAllocator, gErrorCallback);
    m_pvd = PxCreatePvd(*m_foundation);

    physx::PxTolerancesScale scale;
    m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, scale, true, m_pvd);

    physx::PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
    sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    m_scene = m_physics->createScene(sceneDesc);

    m_defaultMaterial = m_physics->createMaterial(0.5f, 0.5f, 0.6f);

    m_controllerManager = PxCreateControllerManager(*m_scene);

#endif
}

physics::PhysicsController& physics::PhysicsController::instance()
{
    static physics::PhysicsController instance;
    return instance;
}

void physics::PhysicsController::simulate(float deltaTime)
{
#ifdef VELIX_USE_PHYSX
    if (!m_scene)
        return;

    m_scene->simulate(deltaTime);
    m_scene->fetchResults(true);

#endif //VELIX_USE_PHYSX
}

void physics::PhysicsController::release()
{
#ifdef VELIX_USE_PHYSX
    if (m_scene)
        m_scene->release();
    if (m_physics)
        m_physics->release();
    if (m_foundation)
        m_foundation->release();

#endif //VELIX_USE_PHYSX
}

physx::PxControllerManager* physics::PhysicsController::getControllerManager() const
{
#ifdef VELIX_USE_PHYSX
    return m_controllerManager;
#else
    return nullptr;
#endif
}

physx::PxMaterial* physics::PhysicsController::getDefaultMaterial() const
{
#ifdef VELIX_USE_PHYSX
    return m_defaultMaterial;
#else
    return nullptr;
#endif
}

physx::PxScene* physics::PhysicsController::getScene() const
{
#ifdef VELIX_USE_PHYSX
    return m_scene;
#else
    return nullptr;
#endif
}

physx::PxRigidDynamic* physics::PhysicsController::addDynamicActor(std::shared_ptr<GameObject> actor) const
{
#ifdef VELIX_USE_PHYSX
    if (!m_physics)
    {
        ELIX_LOG_ERROR("Physics is not initialized");
        return nullptr;
    }

    if (!m_scene)
    {
        ELIX_LOG_ERROR("Scene is not initialized");
        return nullptr;
    }

    const physx::PxTransform transform(physx::PxVec3(actor->getPosition().x, actor->getPosition().y, actor->getPosition().z));
    physx::PxRigidDynamic* rigidBody = m_physics->createRigidDynamic(transform);

    if (!rigidBody)
    {
        ELIX_LOG_ERROR("Failed to create rigid body");
        return nullptr;
    }

    const physx::PxMaterial* material = getDefaultMaterial();

    if (!material)
    {
        ELIX_LOG_ERROR("Default material missing");
        return nullptr;
    }

    physx::PxShape* shape = m_physics->createShape(physx::PxBoxGeometry(actor->getScale().x * 0.5f, actor->getScale().y * 0.5f, actor->getScale().z * 0.5f), *material);

    if (!shape)
    {
        ELIX_LOG_ERROR("Failed to create collision shape");
        rigidBody->release();
        return nullptr;
    }

    shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
    shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
    physx::PxFilterData filterData;
    filterData.word0 = actor->getLayerMask();
    shape->setSimulationFilterData(filterData);
    shape->setQueryFilterData(filterData);

    if (!rigidBody->attachShape(*shape))
    {
        ELIX_LOG_ERROR("Failed to attach shape");
        shape->release();
        rigidBody->release();
        return nullptr;
    }

    rigidBody->userData = actor.get();

    physx::PxRigidBodyExt::updateMassAndInertia(*rigidBody, 10.0f);

    m_scene->addActor(*rigidBody);

    return rigidBody;

#else
    ELIX_LOG_ERROR("Physics disabled: addDynamicActor returns nullptr");
    return nullptr;

#endif //VELIX_USE_PHYSX
}

physx::PxRigidStatic * physics::PhysicsController::addStaticActor(std::shared_ptr<GameObject> actor)
{
#ifdef VELIX_USE_PHYSX
    if (!m_physics)
    {
        std::cerr << "physics::PhysicsController::addDynamicActor(): physics is not initialized" << std::endl;
        return nullptr;
    }

    if (!m_scene)
    {
        std::cerr << "physics::PhysicsController::addDynamicActor(): scene is not initialized" << std::endl;
        return nullptr;
    }

    const physx::PxMaterial* material = getDefaultMaterial();

    if (!material)
    {
        ELIX_LOG_ERROR("Default material missing");
        return nullptr;
    }

    const physx::PxTransform transform(physx::PxVec3(actor->getPosition().x, actor->getPosition().y, actor->getPosition().z));
    physx::PxRigidStatic* staticBody = m_physics->createRigidStatic(transform);

    if (!staticBody)
    {
        ELIX_LOG_ERROR("Failed to create static body");
        return nullptr;
    }

    physx::PxShape* shape = m_physics->createShape(physx::PxBoxGeometry(actor->getScale().x * 0.5f, actor->getScale().y * 0.5f, actor->getScale().z * 0.5f),  *material);

    if (!shape)
    {
        ELIX_LOG_ERROR("Failed to create collision shape");
        staticBody->release();
        return nullptr;
    }

    shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true); // Enable raycasting
    shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);   // Optional if you want physics
    physx::PxFilterData filterData;
    filterData.word0 = actor->getLayerMask();
    shape->setSimulationFilterData(filterData);
    shape->setQueryFilterData(filterData);

    if (!staticBody->attachShape(*shape))
    {
        ELIX_LOG_ERROR("Failed to attach shape");
        shape->release();
        staticBody->release();
        return nullptr;
    }

    staticBody->userData = actor.get();

    m_scene->addActor(*staticBody);

    return staticBody;

#else
    ELIX_LOG_ERROR("Physics disabled: addStaticActor returns nullptr");
    return nullptr;
#endif
}

void physics::PhysicsController::resizeCollider(const glm::vec3 &newSize, std::shared_ptr<GameObject> collider)
{
#ifdef VELIX_USE_PHYSX
    //TODO: Check if actor is in the scene

    const auto rigidBody = collider->getComponent<RigidbodyComponent>();

    if (!rigidBody)
        return;

    const auto actor = rigidBody->getRigidActor();

    if (!actor)
        return;

    physx::PxShape* shapes[8];
    const uint32_t shapeCount = actor->getNbShapes();
    actor->getShapes(shapes, shapeCount);

    for (uint32_t i = 0; i < shapeCount; ++i)
    {
        actor->detachShape(*shapes[i]);
        shapes[i]->release();
    }

    physx::PxBoxGeometry newBox(physx::PxVec3(newSize.x, newSize.y, newSize.z));
    physx::PxMaterial* material = getDefaultMaterial();
    physx::PxShape* newShape = physx::PxRigidActorExt::createExclusiveShape(*actor, newBox, *material);

    newShape->setLocalPose(physx::PxTransform(physx::PxVec3(0, 0, 0)));

    physx::PxFilterData filterData;
    filterData.word0 = collider->getLayerMask();
    newShape->setSimulationFilterData(filterData);
    newShape->setQueryFilterData(filterData);

#else
    ELIX_LOG_ERROR("Physics disabled: resizeCollider does nothing");
#endif //VELIX_USE_PHYSX
}

// physx::PxFilterData filterData;
// filterData.word0 = actor->getLayerMask();     // The layer this object belongs to
// filterData.word1 = common::LayerMask::DEFAULT | common::LayerMask::GROUND;  // What this object can collide/interact with
//
// shape->setSimulationFilterData(filterData);
// shape->setQueryFilterData(filterData);

