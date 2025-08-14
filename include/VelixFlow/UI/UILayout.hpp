#ifndef UI_LAYOUT_HPP
#define UI_LAYOUT_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include <memory>

ELIX_NAMESPACE_BEGIN

namespace ui
{
    class UIWidget;

    class UILayout
    {
    public:
        void setSpacing(float spacing) {spacing_ = spacing;}
        float getSpacing() const {return spacing_;}

        virtual void apply(UIWidget* widget) = 0;

        virtual ~UILayout() = default;
    protected:
        float spacing_{1.0f};
    };

} //namespace ui

ELIX_NAMESPACE_END

#endif //UI_LAYOUT_HPP