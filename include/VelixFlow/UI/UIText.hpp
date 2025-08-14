#ifndef UI_TEXT_HPP
#define UI_TEXT_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/UI/UIWidget.hpp" 
#include "VelixFlow/UI/UIFont.hpp"

#include <string>

ELIX_NAMESPACE_BEGIN

namespace ui
{
    class UIText : public UIWidget
    {
    public:
        void setFont(const std::shared_ptr<UIFont>& font);
        void setText(const std::string& text);
        void setScale(float scale);

        float getScale() const;
        const std::string& getText() const;
        std::shared_ptr<UIFont> getFont();
    private:
        std::string m_text;
        glm::vec3 m_color{1.0f};
        float m_scale{1.0f};
        std::shared_ptr<UIFont> m_font{nullptr};
    };
} //namespace ui

ELIX_NAMESPACE_END

#endif //UI_TEXT_HPP