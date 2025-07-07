#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Render.hpp"
#include <vector>
#include <memory>
#include "CameraComponent.hpp"

namespace elix
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

        std::shared_ptr<elix::FrameBuffer> initFbo(int width, int height);
        std::shared_ptr<elix::FrameBuffer> getFbo() const;



        const FrameData& updateFrameData(elix::CameraComponent* camera);
        
        void renderPath(int priority, const FrameData& frameData, Scene* scene = nullptr);
        void renderPath(const std::string& name, const FrameData& frameData, Scene* scene = nullptr);
        void renderAll(const FrameData& frameData, Scene* scene = nullptr);

        void removeRenderPath(const std::string& name);
    private:
        std::shared_ptr<elix::FrameBuffer> m_fbo{nullptr};

        void sortRenderPassesByPriority();

        FrameData m_frameData;

        std::vector<std::unique_ptr<elix::IRender>> m_renderPasses;
    };
} //namespace elix

#endif //RENDERER_HPP
