#include "VelixFlow/UI/UIHorizontalBox.hpp"
#include "VelixFlow/Logger.hpp"

#include "VelixFlow/UI/UIWidget.hpp"

ELIX_NAMESPACE_BEGIN

namespace ui
{
    void UIHorizontalBox::apply(UIWidget* parent)
    {
        float totalWidth = 0.0f;

        for (auto& child : parent->getChildren())
            totalWidth += child->getSize().x;

        totalWidth += spacing_ * (parent->getChildren().size() - 1);

        float startX = 0.0f;

        if (parent->getAnchor() == UIAnchor::CENTER)
            startX = (parent->getSize().x - totalWidth) / 2.0f;
        // else if (parent->getAnchor() == UIAnchor::LEFT) {
        //     startX = 0.0f;
        // }
        // else if (parent->getAnchor() == UIAnchor::RIGHT) {
        //     startX = parent->getSize().x - totalWidth;
        // }

        float xOffset = startX;

        for (auto& child : parent->getChildren())
        {
            float yPos = (parent->getSize().y - child->getSize().y) / 2.0f;
            child->setPosition({parent->getPosition().x + xOffset, parent->getPosition().y + yPos});
            xOffset += child->getSize().x + spacing_;
        }
    }
} //namespace ui

ELIX_NAMESPACE_END