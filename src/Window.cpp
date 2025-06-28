#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include "Window.hpp"
#include "Logger.hpp"
#include <stdexcept>


window::Window::Window(const std::string& windowTitle, const WindowData& windowData) : m_windowName(windowTitle), m_currentWindowData(windowData)
{
    m_window = glfwCreateWindow(m_currentWindowData.width, m_currentWindowData.height, m_windowName.c_str(), nullptr, nullptr);
    
    if (!m_window)
    {
        glfwTerminate();
        throw std::runtime_error("Window::Window(): could not create a window");
    }

    glfwMakeContextCurrent(m_window);

    // glfwSetWindowUserPointer(m_window, this);
    // glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* win, int width, int height) {
    //     auto* self = static_cast<MainWindow*>(glfwGetWindowUserPointer(win));
    //     self->m_currentWindowData.width = width;
    //     self->m_currentWindowData.height = height;
    //     self->setViewport(0, 0, width, height);
    // });

    viewport();
}

void window::Window::setVsync(bool enabled)
{
    glfwSwapInterval(enabled);
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

GLFWwindow *window::Window::getOpenGLWindow() const
{
    return m_window;
}

void window::Window::pollEvents()
{
    glfwPollEvents();
}

float window::Window::getTime()
{
    return glfwGetTime();
}

void window::Window::setViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
    viewportX = x;
    viewportY = y;
    viewportWidth = width;
    viewportHeight = height;
    // ELIX_LOG_INFO("Changed viewport to ", width, " ", height);
}

void window::Window::viewport() const
{
    glViewport(0, 0, m_currentWindowData.width, m_currentWindowData.height);
    viewportX = 0;
    viewportY = 0;
    viewportWidth = m_currentWindowData.width;
    viewportHeight = m_currentWindowData.height;

    // ELIX_LOG_INFO("Changed viewport to ", m_currentWindowData.width, " ", m_currentWindowData.height);
}

void window::Window::swapBuffers() const
{
    glfwSwapBuffers(m_window);
}

void window::Window::clear(ClearFlag flags)
{
    GLbitfield mask = 0;
    if (flags & ClearFlag::COLOR_BUFFER_BIT)   mask |= GL_COLOR_BUFFER_BIT;
    if (flags & ClearFlag::DEPTH_BUFFER_BIT)   mask |= GL_DEPTH_BUFFER_BIT;
    if (flags & ClearFlag::STENCIL_BUFFER_BIT) mask |= GL_STENCIL_BUFFER_BIT;

    glClear(mask);
}

void window::Window::setCullMode(CullMode mode)
{
    glCullFace(mode == CullMode::BACK ? GL_BACK : GL_FRONT);
}

void window::Window::setDepthFunc(bool enabled)
{
    glDepthFunc(enabled ? GL_LEQUAL : GL_LESS);
}

void window::Window::lineWidth(float lineWidth)
{
    glLineWidth(lineWidth);
}


void window::Window::setSize(int width, int height)
{
    m_currentWindowData.width = width;
    m_currentWindowData.height = height;

    glfwSetWindowSize(m_window, width, height);
}
