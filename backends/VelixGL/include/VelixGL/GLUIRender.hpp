#ifndef UI_RENDER_HPP
#define UI_RENDER_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/Interface/IRenderPass.hpp"
#include "VelixFlow/UI/UIElement.hpp"

ELIX_NAMESPACE_BEGIN

namespace render
{
    class GLUIRender : public IRenderPass
    {
    public:
        GLUIRender();
        window::ClearFlag getClearFlag() override;
        bool shouldExecute() const override;
        std::string getName() const override;
        void render(const FrameData& frameData, Scene* scene = nullptr) override;
        int getPriority() const override;
        ~GLUIRender() override = default;
    private:
        unsigned int m_VAO, m_VBO;
        void drawUIElements(const FrameData& frameData, ui::UIElement* element);
        void drawTextElements(const FrameData& frameData, ui::UIElement* element);
    };
}
ELIX_NAMESPACE_END

#endif //UI_RENDER_HPP