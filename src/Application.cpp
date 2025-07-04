#include <glad/glad.h>

#include "Application.hpp"

#ifdef _WIN32
    #include <windows.h>
    #include <dbghelp.h>
#endif

#ifndef _WIN32
    #include <execinfo.h>
    #include <dlfcn.h>
#endif

#include <stdexcept>
#include <GLFW/glfw3.h>

#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Logger.hpp"
#include <csignal>
#include <cstdlib>
#include "Physics.hpp"
#include "AudioSystem.hpp"

float elix::Application::getDeltaTime() const
{
    return m_deltaTime;
}

elix::Renderer* elix::Application::getRenderer() const
{
    return m_renderer.get();
}

elix::CameraComponent* elix::Application::getCamera() const
{
    return m_camera.get();
}

Scene* elix::Application::getScene() const
{
    return m_scene.get();
}

void elix::Application::update()
{
    const float currentFrame = window::Window::getTime();
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;

    window::Window::pollEvents();

    physics::PhysicsController::instance().simulate(m_deltaTime);
    
    m_camera->update(m_deltaTime);

    m_scene->update(m_deltaTime);

}
void elix::Application::endRender()
{
    m_mainWindow->swapBuffers();
}

void elix::Application::render()
{
    if(!m_scene)
        return;
    
    const auto& frameData = m_renderer->updateFrameData(m_camera.get());

    m_renderer->renderAll(frameData, m_scene.get());
}

#ifdef _DEBUG
void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                              GLsizei length, const GLchar* message, const void* userParam)
{
    std::string msg = std::string("[GL CALLBACK] ") + message;

    if (severity == GL_DEBUG_SEVERITY_HIGH)
        ELIX_LOG_ERROR(msg);
    else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
        ELIX_LOG_WARN(msg);
    else if (severity == GL_DEBUG_SEVERITY_LOW)
        ELIX_LOG_INFO(msg);
}

#endif

void signalHandler(int signal)
{
#if defined(_WIN32)
    void* stack[50];
    unsigned short frames;
    SYMBOL_INFO* symbol;
    HANDLE process = GetCurrentProcess();

    SymInitialize(process, NULL, TRUE);
    frames = CaptureStackBackTrace(0, 50, stack, NULL);
    symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    ELIX_LOG_ERROR("Signal ", signal);

    for (unsigned int i = 0; i < frames; i++) {
        SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);

        // ELIX_LOG_ERROR(frames - i - 1, symbol->Name, symbol->Address)
        // ELIX_LOG_ERROR("%i: %s - 0x%0X", frames - i - 1, symbol->Name, symbol->Address);
    }

    free(symbol);
#elif defined(__linux__) || defined(__APPLE__)
    void* array[25];
    size_t size = backtrace(array, 25);
    char** strings = backtrace_symbols(array, size);

    ELIX_LOG_ERROR("Signal ", signal);

    for (size_t i = 0; i < size; i++)
        ELIX_LOG_ERROR(strings[i]);

    free(strings);
#else
    //It should not happened.... Maybe...
    ELIX_LOG_ERROR("Signal %d received. (No backtrace on this platform)", signal);
#endif

    elix::Application::shutdownCore();

    std::exit(signal);
}

void terminateHandler()
{
    ELIX_LOG_ERROR("Unhandled exception. Terminating...");

    elix::Application::shutdownCore();

    std::abort();
}

void elix::Application::shutdownCore()
{

    glfwTerminate();
}

std::unique_ptr<elix::Application> elix::Application::createApplication()
{
    auto app = std::make_unique<elix::Application>();

    app->init();

    return std::move(app);
}

window::Window* elix::Application::getWindow() const
{
    return m_mainWindow.get();
}

void elix::Application::init()
{
    if (!glfwInit())
        throw std::runtime_error("Application::initializeCore(): Failed to initialize glfw");

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

    m_mainWindow = std::make_unique<window::Window>();

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        throw std::runtime_error("Application::initializeCore(): Failed to initialize GLAD");

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version  = glGetString(GL_VERSION);

    ELIX_LOG_INFO(reinterpret_cast<const char *>(renderer));
    ELIX_LOG_INFO(reinterpret_cast<const char *>(version));

    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    const std::string ver = std::to_string(major) + "." + std::to_string(minor);

    ELIX_LOG_INFO("Context version: ", ver);

#ifdef __APPLE__
    if (major < 4 || (major == 4 && minor < 1))
        throw std::runtime_error("OpenGL 4.1+ required on macOS");
#else
    if (major < 4 || (major == 4 && minor < 6))
        throw std::runtime_error("OpenGL 4.6+ required");
#endif
    
    glfwSetKeyCallback(m_mainWindow->getOpenGLWindow(), input::KeysManager::keyCallback);
    glfwSetMouseButtonCallback(m_mainWindow->getOpenGLWindow(), input::MouseManager::mouseButtonCallback);
    glfwSetCursorPosCallback(m_mainWindow->getOpenGLWindow(), input::MouseManager::mouseCallback);
    glfwSetScrollCallback(m_mainWindow->getOpenGLWindow(), input::MouseManager::scrollCallback);
    glfwSetInputMode(m_mainWindow->getOpenGLWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL); //GLFW_CURSOR_NORMAL | GLFW_CURSOR_DISABLED

#ifdef _WIN32
    SetProcessDPIAware();
#endif

#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                              GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

     glEnable(GL_DEPTH_TEST);
     glEnable(GL_CULL_FACE);
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     glDepthFunc(GL_LESS);
     glEnable(GL_STENCIL_TEST);
     glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
     glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

     std::signal(SIGSEGV, signalHandler);
     std::signal(SIGABRT, signalHandler);
     std::signal(SIGFPE,  signalHandler);
     std::signal(SIGTERM, signalHandler);

// SIGKILL is not available on Windows
#ifndef _WIN32
    std::signal(SIGKILL, signalHandler);
#endif

    std::set_terminate(terminateHandler);

    m_renderer = std::make_unique<elix::Renderer>();
    m_camera = std::make_unique<elix::CameraComponent>();
    m_scene = std::make_unique<Scene>();

    physics::PhysicsController::instance().init();

    audio::AudioSystem::instance().init();
}
