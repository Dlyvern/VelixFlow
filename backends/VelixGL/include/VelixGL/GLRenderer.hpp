#ifndef GL_RENDERER_HPP
#define GL_RENDERER_HPP

#include "VelixFlow/RenderAPI/Interface/IRenderer.hpp"
#include "VelixGL/OpenGLContext.hpp"
#include "VelixFlow/RenderAPI/Renderer.hpp"

ELIX_NAMESPACE_BEGIN

class GLRenderer : public IRenderer
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

    void swapBuffers(window::Window* window) override;
    void pollEvents() override;

    void addRenderPath(const std::string& name, window::Window* window) override;

private:
    OpenGLContext m_openglContext;
    render::Renderer m_renderer;
};

ELIX_NAMESPACE_END


#endif //GL_RENDERER_HPP