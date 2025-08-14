#ifndef UI_DIALOG_HPP
#define UI_DIALOG_HPP

#include "VelixFlow/UI/UIWidget.hpp"

ELIX_NAMESPACE_BEGIN

namespace ui
{
    class UIDialog : public UIWidget
    {
    public:
        void update(float deltaTime) override;
    };
    
} //namespace ui


ELIX_NAMESPACE_END


#endif //UI_DIALOG_HPP