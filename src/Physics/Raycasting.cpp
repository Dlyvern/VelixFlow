#include "VelixFlow/Physics/Raycasting.hpp"
#include "VelixFlow/LayerMask.hpp"

bool physics::raycasting::shoot(const Ray &ray, RaycastingResult &result)
{
#ifdef VELIX_USE_PHYSX
    glm::vec3 direction = glm::normalize(ray.direction);

    physx::PxQueryFilterData filterData;
    filterData.flags = physx::PxQueryFlag::eSTATIC | physx::PxQueryFlag::eDYNAMIC;
    filterData.data.word0 = elix::LayerMask::DEFAULT | elix::LayerMask::INTERACTABLE | elix::LayerMask::GROUND;

    physx::PxRaycastBuffer hit;

    const auto scene = physics::PhysicsController::instance().getScene();

    bool status = scene->raycast({ray.origin.x, ray.origin.y, ray.origin.z}, {direction.x, direction.y, direction.z}, ray.maxDistance, hit, physx::PxHitFlag::eDEFAULT, filterData);

    result.hit = hit;

    return status && hit.hasBlock;

#else
    return false;

#endif //VELIX_USE_PHYSX
}
