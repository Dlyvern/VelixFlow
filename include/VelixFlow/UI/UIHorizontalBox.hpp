#ifndef UI_HORIZONTAL_BOX_HPP
#define UI_HORIZONTAL_BOX_HPP

#include "VelixFlow/UI/UILayout.hpp"

ELIX_NAMESPACE_BEGIN

namespace ui
{
    class UIHorizontalBox : public UILayout
    {
    public:
        void apply(UIWidget* parent) override;
        
    };

} //namespace ui

ELIX_NAMESPACE_END

#endif //UI_HORIZONTAL_BOX_HPP