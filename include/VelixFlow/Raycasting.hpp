#ifndef RAYCASTING_HPP
#define RAYCASTING_HPP

#include "Physics.hpp"

namespace physics::raycasting
{
    struct RaycastingResult
    {
    public:
        glm::vec3 point;
        float distance;
        physx::PxRaycastBuffer hit;
    };

    struct Ray
    {
    public:
        glm::vec3 origin{0.0f};
        glm::vec3 direction{0.0f, 0.0f, 1.0f};
        float maxDistance{0.1f};
    };

    bool shoot(const Ray& ray, RaycastingResult& result);
} // namespace physics::raycasting


#endif //RAYCASTING_HPP