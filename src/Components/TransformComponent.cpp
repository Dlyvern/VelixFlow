#include "VelixFlow/Components/TransformComponent.hpp"
#include <glm/gtc/matrix_transform.hpp>

ELIX_NAMESPACE_BEGIN

namespace components
{
    void TransformComponent::setRotation(const glm::vec3 &axis)
    {
        m_rotation = axis;

        m_isTransformMatrixDirty = true;

        transformationChanged.emit(getTransformMatrix());
    }

    void TransformComponent::setPosition(const glm::vec3 &position)
    {
        m_position = position;

        m_isTransformMatrixDirty = true;

        positionChanged.emit(position);

        transformationChanged.emit(getTransformMatrix());
    }

    void TransformComponent::setScale(const glm::vec3 &scale)
    {
        m_scale = scale;
        m_isTransformMatrixDirty = true;
        
        transformationChanged.emit(getTransformMatrix());
    }

    glm::vec3 TransformComponent::getPosition() const
    {
        return m_position;
    }

    glm::vec3 TransformComponent::getScale() const
    {
        return m_scale;
    }

    glm::vec3 TransformComponent::getRotation() const
    {
        return m_rotation;
    }

    glm::mat4 TransformComponent::getTransformMatrix()
    {
        if (!m_isTransformMatrixDirty)
            return m_transformMatrix;

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_position);

        glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
        glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
        glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(0, 0, 1));
        glm::mat4 rotation = rotY * rotX * rotZ;

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_scale);

        m_transformMatrix = translation * rotation * scale;

        m_isTransformMatrixDirty = false;

        return m_transformMatrix;
    }
}

ELIX_NAMESPACE_END