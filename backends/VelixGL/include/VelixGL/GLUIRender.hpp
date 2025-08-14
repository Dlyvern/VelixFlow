#ifndef UI_RENDER_HPP
#define UI_RENDER_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/Interface/IRenderPass.hpp"
#include "VelixFlow/UI/UIWidget.hpp"
#include "VelixGL/GLFontCache.hpp"

ELIX_NAMESPACE_BEGIN

namespace render
{
    class GLUIRender : public IRenderPass
    {
    public:
        GLUIRender();
        bool shouldExecute() const override;
        std::string getName() const override;
        void render(const FrameData& frameData, Scene* scene = nullptr) override;
        int getPriority() const override;
        ~GLUIRender() override = default;
    private:
        unsigned int m_VAO, m_VBO;
        void drawUIElements(const FrameData& frameData, ui::UIWidget* element);
        void drawTextElements(const FrameData& frameData, ui::UIWidget* element);

        unsigned int m_vao;
        unsigned int m_vbo;

        std::shared_ptr<GLFontCache> m_fontCache{nullptr};
    };
}

ELIX_NAMESPACE_END

#endif //UI_RENDER_HPP