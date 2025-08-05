#include "VelixFlow/Components/LightComponent.hpp"
#include "VelixFlow/GameObject.hpp"
#include "VelixFlow/Components/TransformComponent.hpp"

ELIX_NAMESPACE_BEGIN

namespace components
{
    LightComponent::LightComponent(const std::shared_ptr<lighting::Light> &light) : m_light(light) {}

    void LightComponent::update(float deltaTime) {}

    std::shared_ptr<lighting::Light> LightComponent::getLight()
    {
        return m_light;
    }

    void LightComponent::setLocalOffset(const glm::vec3& offset)
    {
        m_localOffset = offset;
        updateLightTransform();
    }

    const glm::vec3& LightComponent::getLocalOffset() const
    {
        return m_localOffset;
    }

    glm::vec3 LightComponent::getWorldPosition() const
    {
        if(auto owner = this->getOwner())
            return owner->getComponent<TransformComponent>()->getTransformMatrix() * glm::vec4(m_localOffset, 1.0f);

        return m_light->position;
    }

    void LightComponent::setOwner(GameObject *owner)
    {
        IComponent::setOwner(owner);

        owner->getComponent<TransformComponent>()->transformationChanged.connect(std::bind(&LightComponent::onTransformationOwnerChanged, this, std::placeholders::_1));

        updateLightTransform();
    }

    void LightComponent::updateLightTransform()
    {
        if(const auto owner = this->getOwner())
        {
            const glm::mat4 transform = owner->getComponent<TransformComponent>()->getTransformMatrix();

            m_light->position = transform * glm::vec4(m_localOffset, 1.0f);

            glm::vec3 forward = glm::normalize(glm::vec3(transform[2]));

            m_light->direction = -forward;
        }
    }

    void LightComponent::destroy()
    {
        IComponent::destroy();
        // LightManager::instance().removeLight(&m_light);
    }

    void LightComponent::onTransformationOwnerChanged(const glm::mat4 &transformation)
    {
        updateLightTransform();
    }

} //namespace components

ELIX_NAMESPACE_END