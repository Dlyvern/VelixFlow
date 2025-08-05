#ifndef UI_BUTTON_HPP
#define UI_BUTTON_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "UIElement.hpp"
#include "UIText.hpp"
#include <memory>
#include "VelixFlow/Signal.hpp"

ELIX_NAMESPACE_BEGIN

namespace ui
{
    class UIButton : public UIElement
    {
    public:
        Signal<int> onClicked;

        UIButton();

        UIButton(const std::string& text);

        void update(float deltaTime);

        void updateLayout(const glm::vec2& screenSize) override;

        void setLabel(const std::string& label);

        void draw(const glm::mat4& projection, const glm::mat4& flippedProjection) override;

        std::shared_ptr<UIText> getText() const;

    private:
        std::shared_ptr<UIText> m_text;
        bool m_isHovered;
    };
} // namespace ui


ELIX_NAMESPACE_END

#endif //UI_BUTTON_HPP