#ifndef LIGHT_COMPONENT_HPP
#define LIGHT_COMPONENT_HPP

#include <glm/mat4x4.hpp>

#include "Component.hpp"
#include "Light.hpp"
#include <memory>

class LightComponent final : public Component
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

#endif //LIGHT_COMPONENT_HPP
