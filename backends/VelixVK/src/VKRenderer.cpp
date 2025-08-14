#include "VelixVK/VKRenderer.hpp"

namespace 
{
    static std::unordered_map<GLFWwindow*, std::function<void(GLFWwindow*, int, int, int, int)>> g_keyCallbacks;

    static void keyCallbackDispatch(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        auto it = g_keyCallbacks.find(window);
        if (it != g_keyCallbacks.end())
        {
            it->second(window, key, scancode, action, mods);
        }
    }

    static std::unordered_map<GLFWwindow*, std::function<void(GLFWwindow *window, int button, int action, int mods)>> g_mouseButtonCallbacks;

    static void mouseCallbackButtonDispatch(GLFWwindow* window, int button, int action, int mods)
    {
        auto it = g_mouseButtonCallbacks.find(window);

        if(it != g_mouseButtonCallbacks.end())
        {
            it->second(window, button, action, mods);
        }
    }

    static std::unordered_map<GLFWwindow*, std::function<void(GLFWwindow *window, double x, double y)>> g_mousePositionCallbacks;

    static void mousePositionCallbackDispatch(GLFWwindow* window, double x, double y)
    {
        auto it = g_mousePositionCallbacks.find(window);

        if(it != g_mousePositionCallbacks.end())
        {
            it->second(window, x, y);
        }
    }
}

ELIX_NAMESPACE_BEGIN

void VKRenderer::init(window::Window*& window)
{
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize glfw");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = new window::Window();

    m_context.init(window->getGLFWWindow());
}

void VKRenderer::shutdown()
{
    glfwTerminate();
    m_context.cleanup();
}

void VKRenderer::addRenderPath(const std::string& name, window::Window* window, Scene* scene)
{

}

void VKRenderer::renderSceneWithPath(const render::FrameData& frameData, Scene* scene, const std::string& pathName)
{
    
}

const render::FrameData& VKRenderer::updateFrameData(components::CameraComponent* camera, int windowWidth, int windowHeight)
{
    return m_renderer.updateFrameData(camera, windowWidth, windowHeight);
}

void VKRenderer::setKeyCallback(const std::function<void(GLFWwindow *window, int key, int scancode, int action, int mods)>& function, window::Window* window)
{
    g_keyCallbacks[window->getGLFWWindow()] = function;

    glfwSetKeyCallback(window->getGLFWWindow(), keyCallbackDispatch);
}

void VKRenderer::setMousePositionCallback(const std::function<void(GLFWwindow* window, double x, double y)>& function, window::Window* window)
{
    g_mousePositionCallbacks[window->getGLFWWindow()] = function;
    glfwSetCursorPosCallback(window->getGLFWWindow(), mousePositionCallbackDispatch);
}

void VKRenderer::setMouseButtonCallback(const std::function<void(GLFWwindow *window, int button, int action, int mods)>& function, window::Window* window)
{
    g_mouseButtonCallbacks[window->getGLFWWindow()] = function;

    glfwSetMouseButtonCallback(window->getGLFWWindow(), mouseCallbackButtonDispatch);
}


void VKRenderer::renderScene(const render::FrameData& frameData, Scene* scene)
{
    m_renderer.renderAll(frameData, scene);
}

IRenderContext* VKRenderer::getContext()
{
    return &m_context;
}

void VKRenderer::swapBuffers(window::Window* window)
{
    window->swapBuffers();
}

void VKRenderer::pollEvents()
{
    window::Window::pollEvents();
}


ELIX_NAMESPACE_END

extern "C" elix::IRenderer* createRenderer()
{
    return new elix::VKRenderer(); 
}
