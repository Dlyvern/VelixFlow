#include "VelixFlow/UI/UIVerticalBox.hpp"
#include "VelixFlow/Logger.hpp"

#include "VelixFlow/UI/UIWidget.hpp"

ELIX_NAMESPACE_BEGIN

namespace ui
{
    void UIVerticalBox::apply(UIWidget* parent)
    {
        float totalHeight = 0.0f;

        for (auto& child : parent->getChildren()) {
            totalHeight += child->getSize().y;
        }
        totalHeight += spacing_ * (parent->getChildren().size() - 1);

        float startY = 0.0f;
        if (parent->getAnchor() == UIAnchor::CENTER) {
            startY = (parent->getSize().y - totalHeight) / 2.0f;
        }
        // else if (parent->getAnchor() == UIAnchor::TOP_LEFT) {
        //     startY = 0.0f;
        // }
        // else if (parent.getAnchor() == UIAnchor::BOTTOM_LEFT) {
        //     startY = parent->getSize().y - totalHeight;
        // }

        float yOffset = startY;
        
        for (auto& child : parent->getChildren())
        {
            float xPos = (parent->getSize().x - child->getSize().x) / 2.0f; // center horizontally
            child->setPosition({parent->getPosition().x + xPos, parent->getPosition().y + yOffset});
            yOffset += child->getSize().y + spacing_;
        }
    }

} //namespace ui

ELIX_NAMESPACE_END