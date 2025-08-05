#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/Components/Component.hpp"
#include "VelixFlow/Signal.hpp"

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

ELIX_NAMESPACE_BEGIN

namespace components
{
    class TransformComponent : public IComponent
    {
    public:
        Signal<const glm::vec3&> positionChanged;
        Signal<const glm::mat4&> transformationChanged;

        void setPosition(const glm::vec3& position);
        void setScale(const glm::vec3& scale);
        void setRotation(const glm::vec3 &axis);

        [[nodiscard]] glm::vec3 getPosition() const;
        [[nodiscard]] glm::vec3 getScale() const;
        [[nodiscard]] glm::vec3 getRotation() const;
        glm::mat4 getTransformMatrix();

    private:
        glm::vec3 m_position{0.0f};
        glm::vec3 m_scale{1.0f};
        glm::vec3 m_rotation{0.0f};

        glm::mat4 m_transformMatrix;

        bool m_isTransformMatrixDirty{true};
    };
} //namespace components

ELIX_NAMESPACE_END

#endif //TRANSFORM_COMPONENT_HPP