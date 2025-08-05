#include "VelixGL/GLRenderer.hpp"
#include <iostream>
#include "VelixGL/ShaderManager.hpp"

#include "VelixGL/GLSceneRender.hpp"

#include <unordered_map>

ELIX_NAMESPACE_BEGIN

void GLRenderer::init(window::Window*& window) 
{
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize glfw");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = new window::Window();

    m_openglContext.init();

    ShaderManager::instance().preLoadShaders();
}

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

void GLRenderer::setKeyCallback(const std::function<void(GLFWwindow *window, int key, int scancode, int action, int mods)>& function, window::Window* window)
{
    g_keyCallbacks[window->getGLFWWindow()] = function;

    glfwSetKeyCallback(window->getGLFWWindow(), keyCallbackDispatch);
}

void GLRenderer::setMousePositionCallback(const std::function<void(GLFWwindow* window, double x, double y)>& function, window::Window* window)
{
    g_mousePositionCallbacks[window->getGLFWWindow()] = function;
    glfwSetCursorPosCallback(window->getGLFWWindow(), mousePositionCallbackDispatch);
}

void GLRenderer::setMouseButtonCallback(const std::function<void(GLFWwindow *window, int button, int action, int mods)>& function, window::Window* window)
{
    g_mouseButtonCallbacks[window->getGLFWWindow()] = function;

    glfwSetMouseButtonCallback(window->getGLFWWindow(), mouseCallbackButtonDispatch);
}

void GLRenderer::addRenderPath(const std::string& name, window::Window* window)
{
    if(name == "GLSceneRender")
    {
        auto path = m_renderer.addRenderPath<render::GLSceneRender>();
        path->setWindow(window);
    }
}

void GLRenderer::swapBuffers(window::Window* window)
{
    window->swapBuffers();
}

const render::FrameData& GLRenderer::updateFrameData(components::CameraComponent* camera, int windowWidth, int windowHeight)
{
    return m_renderer.updateFrameData(camera, windowWidth, windowHeight);
}

void GLRenderer::pollEvents()
{
    window::Window::pollEvents();
}

void GLRenderer::shutdown()
{
    glfwTerminate();
    m_openglContext.cleanup();
}

void GLRenderer::renderScene(const render::FrameData& frameData, Scene* scene) 
{
    m_renderer.renderAll(frameData, scene);
}

IRenderContext* GLRenderer::getContext()
{
    return &m_openglContext;
}


ELIX_NAMESPACE_END

extern "C" elix::IRenderer* createRenderer()
{
    return new elix::GLRenderer(); 
}
