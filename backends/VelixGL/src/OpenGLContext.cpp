#include "VelixGL/OpenGLContext.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "VelixFlow/Logger.hpp"

#include "VelixGL/GLTexture.hpp"
#include "VelixGL/GLMesh.hpp"

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

ELIX_NAMESPACE_BEGIN

std::shared_ptr<mesh::IMesh> OpenGLContext::createMesh(const mesh::MeshData& meshData)
{
    return std::make_shared<GLMesh>(meshData);
}
std::unique_ptr<texture::ITexture> OpenGLContext::createTexture(const std::string& path)
{
    return std::make_unique<elix::texture::GLTexture>();
}

void OpenGLContext::init()
{
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        throw std::runtime_error("OpenGLContext: Failed to initialize GLAD");

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
}

void OpenGLContext::cleanup()
{
    
}

ELIX_NAMESPACE_END