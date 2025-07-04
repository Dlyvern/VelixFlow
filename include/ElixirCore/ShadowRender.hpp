#ifndef SHADOW_RENDER_HPP
#define SHADOW_RENDER_HPP

#include "Render.hpp"
#include "ShadowSystem.hpp"
#include "Light.hpp"

namespace elix
{
    class ShadowRender : public elix::IRender
    {
    public:
        ShadowRender(const std::vector<std::shared_ptr<lighting::Light>>& lights);
        bool shouldExecute() const override;
        std::string getName() const override;
        void render(const elix::FrameData& frameData, Scene* scene = nullptr) override;
        int getPriority() const override;
        ~ShadowRender() override = default;
    private:
        elix::ShadowSystem m_shadowSystem;
    };
} //namespace elix

#endif //SHADOW_RENDER_HPP
