#include "VelixFlow/Components/CameraComponent.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

ELIX_NAMESPACE_BEGIN

namespace components
{
    CameraComponent::CameraComponent()
    {
        updateCameraVectors();
    }

    void CameraComponent::update(float deltaTime)
    {
        updateCameraVectors();
    }

    glm::vec3 CameraComponent::getPosition() const
    {
        return m_position;
    }

    glm::vec3 CameraComponent::getForward() const
    {
        return m_forward;
    }

    glm::vec3 CameraComponent::getUp() const
    {
        return m_up;
    }

    glm::mat4 CameraComponent::getViewMatrix() const
    {
        return glm::lookAt(m_position, m_position + m_forward, m_up);
    }

    float CameraComponent::getPitch() const
    {
        return m_pitch;
    }

    float CameraComponent::getYaw() const
    {
        return m_yaw;
    }

    float CameraComponent::getFOV() const {return m_fov;}
    float CameraComponent::getNear() const {return m_near;};
    float CameraComponent::getFar() const {return m_far;};

    glm::mat4 CameraComponent::getProjectionMatrix() const
    {
        return glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    }

    void CameraComponent::setYaw(float yaw)
    {
        m_yaw = yaw;
        updateCameraVectors();
    }

    void CameraComponent::setPosition(const glm::vec3 &position)
    {
        m_position = position;
    }

    void CameraComponent::setPitch(float pitch)
    {
        m_pitch = glm::clamp(pitch, -89.0f, 89.0f);
        updateCameraVectors();
    }

    void CameraComponent::updateCameraVectors()
    {
        glm::vec3 forward;
        forward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        forward.y = sin(glm::radians(m_pitch));
        forward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

        m_forward = glm::normalize(forward);
        m_right = glm::normalize(glm::cross(m_forward, glm::vec3(0.0f, 1.0f, 0.0f)));
        m_up = glm::normalize(glm::cross(m_right, m_forward));
    }
}

ELIX_NAMESPACE_END