#ifndef VK_SCENE_RENDER_HPP
#define VK_SCENE_RENDER_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/Interface/IRenderPass.hpp"

ELIX_NAMESPACE_BEGIN

namespace render
{
    class VKSceneRender : public IRenderPass
    {
        window::ClearFlag getClearFlag() override;
        bool shouldExecute() const override;
        std::string getName() const override;
        void render(const FrameData& frameData, Scene* scene = nullptr) override;
        int getPriority() const override;
    };
} //namespace render

ELIX_NAMESPACE_END

#endif //VK_SCENE_RENDER_HPP