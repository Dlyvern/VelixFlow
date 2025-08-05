#ifndef IRENDERER_HPP
#define IRENDERER_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/Window.hpp"
#include "VelixFlow/RenderAPI/Interface/IRenderContext.hpp"

#include "VelixFlow/RenderAPI/Renderer.hpp"

#include <string>

#include <functional>

ELIX_NAMESPACE_BEGIN

class IRenderer
{
public:
    virtual void init(window::Window*& window) = 0;

    virtual void shutdown() = 0;

    virtual const render::FrameData& updateFrameData(components::CameraComponent* camera, int windowWidth, int windowHeight) = 0;

    virtual void setKeyCallback(const std::function<void(GLFWwindow *window, int key, int scancode, int action, int mods)>& function, window::Window* window) = 0;

    virtual void setMouseButtonCallback(const std::function<void(GLFWwindow *window, int button, int action, int mods)>& function, window::Window* window) = 0;

    virtual void setMousePositionCallback(const std::function<void(GLFWwindow* window, double x, double y)>& function, window::Window* window) = 0;

    virtual void renderScene(const render::FrameData& frameData, Scene* scene) = 0;

    virtual IRenderContext* getContext() = 0;

    virtual void swapBuffers(window::Window* window) = 0;
    virtual void pollEvents() = 0;

    //TODO Render shit(Like window or FBO)
    virtual void addRenderPath(const std::string& name, window::Window* window) = 0;
};

ELIX_NAMESPACE_END

#endif //IRENDERER_HPP