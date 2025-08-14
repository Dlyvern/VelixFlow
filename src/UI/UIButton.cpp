#include "VelixFlow/UI/UIButton.hpp"
#include "VelixFlow/Input/Mouse.hpp"
#include "VelixFlow/Logger.hpp"

ELIX_NAMESPACE_BEGIN

namespace ui
{
    UIButton::UIButton()
    {
        this->positionChanged.connect([this](glm::vec2 position) { updateTextPosition(); });
        this->sizeChanged.connect([this](glm::vec2 size) { updateTextPosition(); });
    }

    void UIButton::setText(const std::string& text)
    {
        if(!m_text)
        {
            m_text = std::make_shared<UIText>();
            addChild(m_text);
        }
        
        m_text->setText(text);

        updateTextPosition();
    }

    std::shared_ptr<UIText> UIButton::getText()
    {
        return m_text;
    }

    void UIButton::updateTextPosition()
    {
        if(!m_text) return;

        glm::vec2 buttonPos = getPosition();
        glm::vec2 buttonSize = getSize();
        glm::vec2 textSize = m_text->getFont() ? m_text->getFont()->calculateTextSize(m_text->getText(), m_text->getScale()) : glm::vec2(0.0f);

        glm::vec2 textPos;
        textPos.x = buttonPos.x + (buttonSize.x - textSize.x) / 2.0f;
        textPos.y = buttonPos.y + (buttonSize.y - textSize.y) / 2.0f;

        m_text->setPosition(textPos);
    }

    void UIButton::update(float deltaTime)
    {
        UIWidget::update(deltaTime);

        glm::vec2 mousePosition = {input::Mouse.getX(), input::Mouse.getY()};

        glm::vec2 position = getPosition();
        glm::vec2 size = getSize();

        //TODO make AABB class for this(2d and 3d)
        m_isHovered = mousePosition.x >= position.x && mousePosition.x <= position.x + size.x &&
        mousePosition.y >= position.y && mousePosition.y <= position.y + size.y;

        if(m_isHovered && input::Mouse.isLeftButtonJustPressed())
        {
            onClicked.emit(0);
        }

        // this->setAlpha(m_isHovered ? 1.0f : 0.5f);
    }
} //namespace ui

ELIX_NAMESPACE_END