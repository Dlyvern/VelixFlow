#include "LightComponent.hpp"
#include "GameObject.hpp"
#include "LightManager.hpp"

LightComponent::LightComponent(const lighting::Light &light) : m_light(light)
{
    LightManager::instance().addLight(&m_light);
}

void LightComponent::update(float deltaTime) {}

lighting::Light* LightComponent::getLight()
{
    return &m_light;
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
    {
        const glm::mat4& transform = owner->getTransformMatrix();
        return transform * glm::vec4(m_localOffset, 1.0f);
    }

    return m_light.position;
}

void LightComponent::setOwner(GameObject *owner)
{
    Component::setOwner(owner);

    owner->setTransformationChangedCallback(std::bind(&LightComponent::onTransformationOwnerChanged, this, std::placeholders::_1));

    updateLightTransform();
}

void LightComponent::updateLightTransform()
{
    if(const auto owner = this->getOwner())
    {
        const glm::mat4 transform = owner->getTransformMatrix();

        m_light.position = transform * glm::vec4(m_localOffset, 1.0f);

        glm::vec3 forward = glm::normalize(glm::vec3(transform[2]));

        m_light.direction = -forward;
    }
}

void LightComponent::destroy()
{
    Component::destroy();
    LightManager::instance().removeLight(&m_light);
}

void LightComponent::onTransformationOwnerChanged(const glm::mat4 &transformation)
{
    updateLightTransform();
}
