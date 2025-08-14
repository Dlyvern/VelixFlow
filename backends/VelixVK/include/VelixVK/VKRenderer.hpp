#ifndef VK_RENDERER_HPP
#define VK_RENDERER_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/Interface/IRenderer.hpp"
#include "VelixFlow/RenderAPI/Renderer.hpp"
#include "VelixVK/VulkanContext.hpp"

ELIX_NAMESPACE_BEGIN

class VKRenderer : public IRenderer
{
public:
    void init(window::Window*& window) override;

    void shutdown() override;

    IRenderContext* getContext() override;

    const render::FrameData& updateFrameData(components::CameraComponent* camera, int windowWidth, int windowHeight) override;

    void setKeyCallback(const std::function<void(GLFWwindow *window, int key, int scancode, int action, int mods)>& function, window::Window* window) override;

    void setMouseButtonCallback(const std::function<void(GLFWwindow *window, int button, int action, int mods)>& function, window::Window* window) override;

    void setMousePositionCallback(const std::function<void(GLFWwindow* window, double x, double y)>& function, window::Window* window) override;

    void renderScene(const render::FrameData& frameData, Scene* scene) override;
    void renderSceneWithPath(const render::FrameData& frameData, Scene* scene, const std::string& pathName) override;

    void swapBuffers(window::Window* window) override;
    void pollEvents() override;

    void addRenderPath(const std::string& name, window::Window* window, Scene* scene) override;
private:
    render::Renderer m_renderer;
    VulkanContext m_context;
};

ELIX_NAMESPACE_END

#endif //VK_RENDERER_HPP