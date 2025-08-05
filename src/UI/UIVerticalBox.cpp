#include "VelixFlow/UI/UIVerticalBox.hpp"
#include "VelixFlow/Logger.hpp"

ELIX_NAMESPACE_BEGIN

void ui::UIVerticalBox::addChild(std::shared_ptr<UIElement> child)
{
    ui::UIElement::addChild(child);
    child->setLayoutControlled(true);
    // layoutChildren();
}

void ui::UIVerticalBox::updateLayout(const glm::vec2& screenSize)
{
    // ELIX_LOG_INFO("Updating layout of ", name_, " screenSize: ", screenSize.x, " ", screenSize.y);
    // ELIX_LOG_INFO("UIVerticalBox normalizedPosition: ", normalizedPosition_.x, normalizedPosition_.y);
    // ELIX_LOG_INFO("Screen Size: ", screenSize.x, screenSize.y);     

    float totalFixedHeight = 0.0f;
    int flexibleCount = 0;

    for (const auto& child : children_)
    {
        if (child->hasFixedHeightNormalized())
            totalFixedHeight += child->getFixedHeightNormalized() * screenSize.y;
        else
            ++flexibleCount;
    }

    float totalSpacing = m_spacing * std::max(0, (int)children_.size() - 1);
    float flexibleHeight = 0.15f * screenSize.y;
    float flexibleTotal = flexibleHeight * flexibleCount;

    float boxHeight = totalFixedHeight + totalSpacing + flexibleTotal;
    float boxWidth = normalizedSize_.x * screenSize.x;
    setSize({ boxWidth, boxHeight });

    glm::vec2 desiredPos = normalizedPosition_ * screenSize;
    setPosition(desiredPos);

    applyAnchor(screenSize);

    glm::vec2 finalPos = getPosition();
    finalPos.x = glm::clamp(finalPos.x, 0.0f, screenSize.x - boxWidth);
    finalPos.y = glm::clamp(finalPos.y, 0.0f, screenSize.y - boxHeight);
    setPosition(finalPos);

    layoutChildren();

    for (auto& child : children_)
        child->updateLayout(screenSize); 
}

void ui::UIVerticalBox::setSpacing(float spacing)
{
    m_spacing = spacing;
}

float ui::UIVerticalBox::getSpacing() const
{
    return m_spacing;
}

void ui::UIVerticalBox::layoutChildren()
{
    glm::vec2 localSize = getSize();
    glm::vec2 localPos = getPosition();

    size_t childCount = children_.size();

    if (childCount == 0) return;

    float totalFixedHeight = 0.0f;
    int flexibleCount = 0;

    for (const auto& child : children_)
    {
        if (child->hasFixedHeightNormalized())
            totalFixedHeight += child->getFixedHeightNormalized() * localSize.y;
        else
            ++flexibleCount;
    }

    float totalSpacing = m_spacing * (childCount - 1);
    float remainingHeight = localSize.y - totalFixedHeight - totalSpacing;
    float flexibleHeight = flexibleCount > 0 ? remainingHeight / flexibleCount : 0.0f;

    float totalNeededHeight = totalFixedHeight + flexibleHeight * flexibleCount + m_spacing * (childCount - 1);
    if (totalNeededHeight > localSize.y) {
        flexibleHeight = std::max(0.0f, (localSize.y - totalFixedHeight - totalSpacing) / flexibleCount);
    }

    // ELIX_LOG_INFO("LayoutBox size: ", localSize.y, " Needed: ", totalFixedHeight + totalSpacing + flexibleHeight * flexibleCount);

    float currentY = 0.0f;

    for (auto& child : children_)
    {
        float height = child->hasFixedHeightNormalized()
            ? child->getFixedHeightNormalized() * localSize.y
            : flexibleHeight;

        child->setSize({ localSize.x, height });
        child->setPosition({ localPos.x, localPos.y + currentY });
        child->setAnchor(UIAnchor::TopLeft);

        currentY += height + m_spacing;
    }
}


ELIX_NAMESPACE_END