#include "VelixFlow/Window.hpp"
#include "VelixFlow/Logger.hpp"
#include "VelixFlow/Input/Keyboard.hpp"
#include "VelixFlow/Input/Mouse.hpp"

#include <stdexcept>

window::Window::Window(const std::string& windowTitle, const WindowData& windowData) : m_windowName(windowTitle), m_currentWindowData(windowData)
{
    m_window = glfwCreateWindow(m_currentWindowData.width, m_currentWindowData.height, m_windowName.c_str(), nullptr, nullptr);
    
    if (!m_window)
        throw std::runtime_error("Failed to create glfw window");

    glfwMakeContextCurrent(m_window);

    glfwSetWindowUserPointer(m_window, this);

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if(!self)
        {
            ELIX_LOG_ERROR("Resize weird shit");
            return;
        }

        self->m_currentWindowData.height = height;
        self->m_currentWindowData.width = width;
    });

    // glfwSetKeyCallback(m_window, input::KeysManager::keyCallback);
    // glfwSetMouseButtonCallback(m_window, input::MouseManager::mouseButtonCallback);
    // glfwSetCursorPosCallback(m_window, input::MouseManager::mouseCallback);
    // glfwSetScrollCallback(m_window, input::MouseManager::scrollCallback);
    // glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); //GLFW_CURSOR_NORMAL | GLFW_CURSOR_DISABLED
}

bool window::Window::isWindowOpened() const
{
    return !(glfwWindowShouldClose(m_window));
}

int window::Window::getHeight() const
{
    return m_currentWindowData.height;
}

int window::Window::getWidth() const
{
    return m_currentWindowData.width;
}

window::WindowData window::Window::getWindowData() const
{
    return m_currentWindowData;
}

GLFWwindow* window::Window::getGLFWWindow() const
{
    return m_window;
}

void window::Window::pollEvents()
{
    glfwPollEvents();
}

void window::Window::swapBuffers() const
{
    glfwSwapBuffers(m_window);
}

void window::Window::setTitle(const std::string& name)
{
    m_windowName = name;
    glfwSetWindowTitle(m_window, name.c_str());
}

bool window::Window::setWindowIcon(elix::Image& image)
{
    if(!image.getData())
        return false;

    GLFWimage images[1];
    images[0].height = image.getHeight();
    images[0].width = image.getWidth();;
    images[0].pixels = image.getData();

    glfwSetWindowIcon(m_window, 1, images);

    return true;
}

void window::Window::setSize(int width, int height)
{
    m_currentWindowData.width = width;
    m_currentWindowData.height = height;

    glfwSetWindowSize(m_window, width, height);
}
