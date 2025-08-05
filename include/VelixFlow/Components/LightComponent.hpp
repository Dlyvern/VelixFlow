#ifndef LIGHT_COMPONENT_HPP
#define LIGHT_COMPONENT_HPP

#include "VelixFlow/Components/Component.hpp"
#include "VelixFlow/Light.hpp"

#include <memory>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

ELIX_NAMESPACE_BEGIN

namespace components
{
    class LightComponent final : public IComponent
    {
    public:
        explicit LightComponent(const std::shared_ptr<lighting::Light>& light);

        void update(float deltaTime) override;

        std::shared_ptr<lighting::Light> getLight();
        
        void setLocalOffset(const glm::vec3& offset);

        const glm::vec3& getLocalOffset() const;

        glm::vec3 getWorldPosition() const;

        void setOwner(GameObject* owner) override;

        void destroy() override;
    private:
        std::shared_ptr<lighting::Light> m_light{};
        glm::vec3 m_localOffset{0.0f, 0.0f, 0.0f};
        void updateLightTransform();
        void onTransformationOwnerChanged(const glm::mat4& transformation);
    };
} //namespace component

ELIX_NAMESPACE_END

#endif //LIGHT_COMPONENT_HPP
