#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <memory>
#include <algorithm>

#include "VelixFlow/Components/CameraComponent.hpp"
#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/Interface/IRenderPass.hpp"

ELIX_NAMESPACE_BEGIN

namespace render
{
    class Renderer
    {
    public:
        template<typename T, typename... Args>
        T* addRenderPath(Args&&... args)
        {
            auto newPath = std::make_unique<T>(std::forward<Args>(args)...);
            m_renderPasses.emplace_back(std::move(newPath));
            sortRenderPassesByPriority();
            return dynamic_cast<T*>(m_renderPasses.back().get());
        }

        template<typename T>
        T* getRenderPath()
        {
            for(const auto& renderPath : m_renderPasses)
            {
                if(auto path = dynamic_cast<T*>(renderPath.get()))
                    return path;
            }

            return nullptr;
        }

        // std::shared_ptr<FrameBuffer> initFbo(int width, int height);
        // std::shared_ptr<FrameBuffer> getFbo() const;

        const FrameData& updateFrameData(components::CameraComponent* camera, int windowWidth, int windowHeight);

        void renderPath(int priority, const FrameData& frameData, Scene* scene = nullptr);
        void renderPath(const std::string& name, const FrameData& frameData, Scene* scene = nullptr);
        void renderAll(const FrameData& frameData, Scene* scene = nullptr);

        void removeRenderPath(const std::string& name);
    private:
        // std::shared_ptr<FrameBuffer> m_fbo{nullptr};

        void sortRenderPassesByPriority();

        FrameData m_frameData;

        std::vector<std::unique_ptr<IRenderPass>> m_renderPasses;
    };
}

ELIX_NAMESPACE_END

#endif //RENDERER_HPP
