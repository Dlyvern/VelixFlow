#include "VelixFlow/UI/UIText.hpp"

ELIX_NAMESPACE_BEGIN

namespace ui
{    
    void UIText::setScale(float scale)
    {
        m_scale = scale;
    }

    void UIText::setFont(const std::shared_ptr<UIFont>& font)
    {
        m_font = font;
    }

    void UIText::setText(const std::string &text)
    {
        m_text = text;
    }

    std::shared_ptr<UIFont> UIText::getFont()
    {
        return m_font;
    }

    const std::string& UIText::getText() const
    {
        return m_text;
    }

    float UIText::getScale() const
    {
        return m_scale;
    }

} //namespace ui

ELIX_NAMESPACE_END