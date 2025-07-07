#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include "DefaultMacros.hpp"
#include "Texture.hpp"

#include <glm/vec4.hpp>
#include <glm/vec2.hpp>

ELIX_NAMESPACE_BEGIN

namespace ui
{
    class UIElement
    {
    public:
        virtual void update(float deltaTime) {}
        
        void setTexture(Texture* texture) {texture_ = texture;}
        Texture* getTexture() const {return texture_;}
        
        void setColor(const glm::vec4& color) {color_ = color;}
        glm::vec4 getColor() const {return color_;}

        void setPosition(const glm::vec2& position) {position_ = position;}
        glm::vec2 getPosition() const {return position_;}

        void setSize(const glm::vec2& size) {size_ = size;}
        glm::vec2 getSize() const {return size_;}

        void setVisible(bool isVisible) {isVisible_ = isVisible;}
        bool isVisible() const {return isVisible_;}

        void setAlpha(float alpha) {alpha_ = alpha;}
        float getAlpha() const {return alpha_;}

        virtual ~UIElement() = default;
    protected:
        Texture* texture_{nullptr};
        glm::vec4 color_{1.0f};
        glm::vec2 position_{0.0f};
        glm::vec2 size_{100.0f, 100.0f};
        float alpha_{1.0f};
        bool isVisible_{true};
    };
} //namespace ui

ELIX_NAMESPACE_END

#endif //UI_ELEMENT_HPP