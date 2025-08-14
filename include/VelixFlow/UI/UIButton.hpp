#ifndef UI_BUTTON_HPP
#define UI_BUTTON_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/UI/UIWidget.hpp"
#include "VelixFlow/UI/UIText.hpp"
#include "VelixFlow/Signal.hpp"

#include <memory>

ELIX_NAMESPACE_BEGIN

namespace ui
{
    class UIButton : public UIWidget
    {
    public:
        Signal<int> onClicked;

        UIButton();

        void setText(const std::string& text);

        std::shared_ptr<UIText> getText();

        void update(float deltaTime);
    private:
        bool m_isHovered;
        std::shared_ptr<UIText> m_text{nullptr};

        void updateTextPosition();
    };
} // namespace ui


ELIX_NAMESPACE_END

#endif //UI_BUTTON_HPP