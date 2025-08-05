#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/Interface/ITexture.hpp"

#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include "VelixFlow/Logger.hpp"
#include <memory>
#include <vector>

ELIX_NAMESPACE_BEGIN

namespace ui
{
    enum class UIAnchor { TopLeft, Center, BottomRight, TopRight, BottomLeft};

    class UIElement
    {
    public:
        virtual void update(float deltaTime) {}

        virtual void addChild(std::shared_ptr<UIElement> child) {children_.push_back(child);}
        
        void setAnchor(UIAnchor anchor) {anchor_ = anchor;}

        std::vector<std::shared_ptr<UIElement>>& getChildren() { return children_;}

        void setFixedHeightNormalized(float h) { 
            normalizedFixedHeight_ = h; 
            hasNormalizedFixedHeight_ = true; 
        }

        bool hasFixedHeightNormalized() const { return hasNormalizedFixedHeight_; }
        float getFixedHeightNormalized() const { return normalizedFixedHeight_; }       

        void applyAnchor(const glm::vec2& parentSize)
        {
            glm::vec2 position = getPosition();
            glm::vec2 size = getSize();

            switch (anchor_)
            {
                case UIAnchor::Center:
                    position.x += (parentSize.x - size.x) * 0.5f;
                    position.y += (parentSize.y - size.y) * 0.5f;
                    break;
                case UIAnchor::BottomRight:
                    position.x += (parentSize.x - size.x);
                    position.y += (parentSize.y - size.y);
                    break;
                case UIAnchor::TopRight:
                    position.x += (parentSize.x - size.x);
                    break;
                case UIAnchor::BottomLeft:
                    position.y += (parentSize.y - size.y);
                    break;
                case UIAnchor::TopLeft:
                default:
                    break;
            }

            setPosition(position);
        }

        void updateHierarchy();
        
        void setTexture(texture::ITexture* texture) {texture_ = texture;}
        texture::ITexture* getTexture() const {return texture_;}
        
        void setColor(const glm::vec4& color) {color_ = color;}
        glm::vec4 getColor() const {return color_;}

        void setPosition(const glm::vec2& position) 
        {
            position_ = position;
            //  ELIX_LOG_INFO("Set position of ", name_, " to ", position_.x, " ", position_.y);
        }
        glm::vec2 getPosition() const {return position_;}

        void setSize(const glm::vec2& size) {size_ = size;}
        glm::vec2 getSize() const {return size_;}

        void setVisible(bool isVisible) {isVisible_ = isVisible;}
        bool isVisible() const {return isVisible_;}

        void setAlpha(float alpha) {alpha_ = alpha;}
        float getAlpha() const {return alpha_;}

        void setName(const std::string& name) {name_ = name;}
        const std::string& getName() const {return name_; }

        virtual void draw(const glm::mat4& projection, const glm::mat4& flippedProjection) {}

        virtual ~UIElement() = default;

        void setLayoutControlled(bool isControlled) {isLayoutControlled_ = isControlled;}

        virtual void updateLayout(const glm::vec2& screenSize)
        {
            // ELIX_LOG_INFO("Updating layout of ", name_, " screenSize: ", screenSize.x, " ", screenSize.y);

            if (!isLayoutControlled_)
            {
                setPosition(normalizedPosition_ * screenSize);
                setSize(normalizedSize_ * screenSize);
                applyAnchor(screenSize);
            }

            for (auto& child : children_)
                child->updateLayout(screenSize);
        }

        void setNormalizedPosition(const glm::vec2& pos) { normalizedPosition_ = pos; }
        void setNormalizedSize(const glm::vec2& size) { normalizedSize_ = size; }
    protected:  
        bool isLayoutControlled_ = false;
        bool hasNormalizedFixedHeight_ = false;
        float normalizedFixedHeight_ = 0.0f;
        std::vector<std::shared_ptr<UIElement>> children_;
        UIAnchor anchor_{UIAnchor::Center};
        std::string name_{"Undefined"};
        texture::ITexture* texture_{nullptr};
        glm::vec4 color_{1.0f};
        glm::vec2 position_{0.0f};
        glm::vec2 size_{0.0f, 0.0f};

        glm::vec2 normalizedPosition_ = {0.0f, 0.0f};
        glm::vec2 normalizedSize_ = {0.5f, 0.5f};

        float alpha_{1.0f};
        bool isVisible_{true};
    };
} //namespace ui

ELIX_NAMESPACE_END

#endif //UI_ELEMENT_HPP