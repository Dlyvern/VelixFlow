#include "VelixFlow/UI/UIDialog.hpp"

ELIX_NAMESPACE_BEGIN

namespace ui
{
    void UIDialog::update(float deltaTime)
    {
        UIWidget::update(deltaTime);

        if(!isVisible())
            return;

        glm::vec2 mousePos = {input::Mouse.getX(), input::Mouse.getY()};
        glm::vec2 pos = getPosition();
        glm::vec2 size = getSize();

        if (input::Mouse.isLeftButtonJustPressed())
        {
            bool inside = mousePos.x >= pos.x && mousePos.x <= pos.x + size.x &&
                          mousePos.y >= pos.y && mousePos.y <= pos.y + size.y;
            
            if (!inside)
            {
                setVisible(false);
                return;
            }
        }

        // Draggable title bar
        // float titleBarHeight = 30.0f;
        // bool overTitleBar = mousePos.x >= pos.x && mousePos.x <= pos.x + size.x &&
        //                     mousePos.y >= pos.y && mousePos.y <= pos.y + titleBarHeight;

        // if (m_draggable)
        // {
        //     if (overTitleBar && input::Mouse.isLeftButtonJustPressed())
        //     {
        //         m_dragging = true;
        //         m_dragOffset = mousePos - pos;
        //     }
        //     else if (input::Mouse.isLeftButtonReleased())
        //     {
        //         m_dragging = false;
        //     }

        //     if (m_dragging)
        //     {
        //         setPosition(mousePos - m_dragOffset);
        //     }
        // }
    }

} //namespace ui

ELIX_NAMESPACE_END