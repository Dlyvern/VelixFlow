#ifndef GL_SCENE_RENDER_HPP
#define GL_SCENE_RENDER_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/Interface/IRenderPass.hpp"

ELIX_NAMESPACE_BEGIN

namespace render
{
    class GLSceneRender : public IRenderPass
    {
    public:
        void setSelectedGameObject(GameObject* gameObject);
        void setWindow(window::Window* window);
        bool shouldExecute() const override;
        std::string getName() const override;
        void render(const FrameData& frameData, Scene* scene = nullptr) override;
        int getPriority() const override;
    private:
        GameObject* m_selectedGameObject{nullptr};
        window::Window* m_window{nullptr};
    };
}

ELIX_NAMESPACE_END

#endif //GL_SCENE_RENDER_HPP