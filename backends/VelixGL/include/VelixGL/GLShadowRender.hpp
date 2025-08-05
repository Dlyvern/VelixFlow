#ifndef SHADOW_RENDER_HPP
#define SHADOW_RENDER_HPP

#include "VelixFlow/RenderAPI/Interface/IRenderPass.hpp"
#include "VelixGL/ShadowSystem.hpp"
#include "VelixFlow/Light.hpp"
#include "VelixFlow/DefaultMacros.hpp"

ELIX_NAMESPACE_BEGIN

namespace render
{
    class GLShadowRender : public IRenderPass
    {
    public:
        GLShadowRender(const std::vector<std::shared_ptr<lighting::Light>>& lights);
        bool shouldExecute() const override;
        std::string getName() const override;
        void render(const FrameData& frameData, Scene* scene = nullptr) override;
        int getPriority() const override;
        ~GLShadowRender() override = default;
    private:
        ShadowSystem m_shadowSystem;
    };
}
ELIX_NAMESPACE_END
#endif //SHADOW_RENDER_HPP
