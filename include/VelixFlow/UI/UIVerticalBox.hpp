#ifndef UI_VERTICAL_BOX_HPP
#define UI_VERTICAL_BOX_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "UILayout.hpp"

ELIX_NAMESPACE_BEGIN

namespace ui
{
    class UIVerticalBox : public UILayout
    {
    public:
        void apply(UIWidget* widget) override;
    private:
    };
} //namespace ui

ELIX_NAMESPACE_END

#endif //UI_VERTICAL_BOX_HPP