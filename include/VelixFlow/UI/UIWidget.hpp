#ifndef UI_WIDGET_HPP
#define UI_WIDGET_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/Interface/ITexture.hpp"
#include "VelixFlow/UI/UIAnchor.hpp"

#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include "VelixFlow/Logger.hpp"
#include <memory>
#include <vector>

#include "VelixFlow/Signal.hpp"

#include "VelixFlow/UI/UILayout.hpp"

#include "VelixFlow/Input/Mouse.hpp"

ELIX_NAMESPACE_BEGIN

namespace ui
{
    class UIWidget
    {
    public:
        Signal<glm::vec2> positionChanged;
        Signal<glm::vec2> sizeChanged;
        
        virtual void update(float deltaTime)
        {
            handleMouseInput();

            if(layout_)
                layout_->apply(this);
            
            for(const auto& child : children_)
                child->update(deltaTime);
        }

        void setLayout(std::shared_ptr<UILayout> layout) { layout_ = layout; }
        std::shared_ptr<UILayout> getLayout() const { return layout_; }

        void addChild(std::shared_ptr<UIWidget> child) {children_.push_back(child);}
        std::vector<std::shared_ptr<UIWidget>>& getChildren() { return children_;}
        
        void setAnchor(UIAnchor anchor) { anchor_ = anchor; }
        UIAnchor getAnchor() const { return anchor_; }

        void setPosition(const glm::vec2& position) { position_ = position; positionChanged.emit(position); }
        glm::vec2 getPosition() const { return position_; }

        void setAlpha(float alpha) { alpha_ = alpha; }
        float getAlpha() const { return alpha_; }

        void setName(const std::string& name) { name_ = name; }
        const std::string& getName() const { return name_; }

        void setTexture(texture::ITexture* texture) { texture_ = texture; }
        texture::ITexture* getTexture() const { return texture_; }
        
        void setColor(const glm::vec4& color) { color_ = color; }
        glm::vec4 getColor() const { return color_; }

        void setSize(const glm::vec2& size) { size_ = size; sizeChanged.emit(size); }
        glm::vec2 getSize() const { return size_; }

        void setVisible(bool isVisible) { isVisible_ = isVisible; }
        bool isVisible() const { return isVisible_; }

        void setResizable(bool resizable) { m_resizable = resizable; }
        bool isResizable() const { return m_resizable; }

        virtual ~UIWidget() = default;
    protected:  
        std::vector<std::shared_ptr<UIWidget>> children_;
        std::shared_ptr<UILayout> layout_{nullptr};
        UIAnchor anchor_{UIAnchor::CENTER};
        std::string name_{"Undefined"};
        texture::ITexture* texture_{nullptr};
        glm::vec4 color_{1.0f};
        glm::vec2 position_{0.0f};
        glm::vec2 size_{0.0f, 0.0f};

        float alpha_{1.0f};
        bool isVisible_{true};

        virtual void handleMouseInput()
        {
            // if(!m_resizable)
            //     return;

            // glm::vec2 mousePos{input::Mouse.getX(), input::Mouse.getY()};
            // glm::vec2 widgetMin = getPosition();
            // glm::vec2 widgetMax = getPosition() + getSize();

            // bool onRight = fabs(mousePos.x - widgetMax.x) <= m_resizeBorder;
            // bool onBottom = fabs(mousePos.y - widgetMax.y) <= m_resizeBorder;

            // // Hover logic: change cursor
            // // if (onRight && onBottom) {
            // //     setMouseCursor(MouseCursor::ResizeDiagonal);
            // // } else if (onRight) {
            // //     setMouseCursor(MouseCursor::ResizeHorizontal);
            // // } else if (onBottom) {
            // //     setMouseCursor(MouseCursor::ResizeVertical);
            // // }

            // // Mouse press start
            // if (input::Mouse.isLeftButtonPressed() && (onRight || onBottom) && !m_isResizing) {
            //     m_isResizing = true;
            //     m_resizeStartMouse = mousePos;
            //     m_resizeStartSize = getSize();
            // }

            // if (m_isResizing && input::Mouse.isLeftButtonPressed()) 
            // {
            //     glm::vec2 delta = mousePos - m_resizeStartMouse;

            //     glm::vec2 newSize = getSize();

            //     if (onRight)
            //         newSize.x = m_resizeStartSize.x + delta.x;
                
            //     if (onBottom)
            //         newSize.y = m_resizeStartSize.y + delta.y;

            //     newSize = glm::max(newSize, glm::vec2(10.0f));

            //     setSize(newSize);
            // }

            // if (!input::Mouse.isLeftButtonReleased())
            //     m_isResizing = false;
        }

    private:
        bool m_resizable{false};
        float m_resizeBorder{6.0f};
        bool m_isResizing{false};
        glm::vec2 m_resizeStartMouse;
        glm::vec2 m_resizeStartSize;
    };
} //namespace ui

ELIX_NAMESPACE_END

#endif //UI_WIDGET_HPP