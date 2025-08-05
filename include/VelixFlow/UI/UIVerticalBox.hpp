#ifndef UI_VERTICAL_BOX_HPP
#define UI_VERTICAL_BOX_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "UIElement.hpp"

ELIX_NAMESPACE_BEGIN

namespace ui
{
    class UIVerticalBox : public UIElement
    {
    public:
        void addChild(std::shared_ptr<UIElement> child) override;

        void layoutChildren();

        void updateLayout(const glm::vec2& screenPosition) override;

        void setSpacing(float spacing);
        float getSpacing() const;
    private:
        float m_spacing{0.01f};
    };
} //namespace ui

ELIX_NAMESPACE_END

#endif //UI_VERTICAL_BOX_HPP