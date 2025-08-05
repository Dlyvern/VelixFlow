#include "VelixFlow/UI/UIButton.hpp"
#include "VelixFlow/Input/Mouse.hpp"
#include "VelixFlow/Logger.hpp"

ELIX_NAMESPACE_BEGIN

ui::UIButton::UIButton() = default;

ui::UIButton::UIButton(const std::string& text)
{
    m_text = std::make_shared<ui::UIText>();
    setLabel(text);
    m_text->setColor({1.0f, 1.0f, 1.0f, 1.0f});
    m_text->setScale(1.0f);
    this->addChild(m_text);
}

void ui::UIButton::setLabel(const std::string& label)
{
    if(!m_text)
        return;

    m_text->setText(label);
    
    m_text->setAnchor(UIAnchor::Center);
}

std::shared_ptr<ui::UIText> ui::UIButton::getText() const
{
    return m_text;
}

void ui::UIButton::update(float deltaTime)
{
    glm::vec2 mousePosition = {input::Mouse.getX(), input::Mouse.getY()};

    glm::vec2 position = getPosition();
    glm::vec2 size = getSize();

    //TODO make AABB class for this(2d and 3d)
    m_isHovered = mousePosition.x >= position.x && mousePosition.x <= position.x + size.x &&
    mousePosition.y >= position.y && mousePosition.y <= position.y + size.y;

    if(m_isHovered && input::Mouse.isLeftButtonPressed())
    {
        onClicked.emit(0);
    }
}

void ui::UIButton::draw(const glm::mat4& projection, const glm::mat4& flippedProjection)
{
    this->setAlpha(m_isHovered ? 1.0f : 0.5f);
}

void ui::UIButton::updateLayout(const glm::vec2& screenSize)
{
    UIElement::updateLayout(screenSize);

    if (m_text)
    {
        glm::vec2 pos = getPosition();
        glm::vec2 size = getSize();
        m_text->setPosition({pos.x + size.x * 0.5f, pos.y + size.y * 0.5f});
        m_text->setAnchor(UIAnchor::Center);
    }
}

ELIX_NAMESPACE_END