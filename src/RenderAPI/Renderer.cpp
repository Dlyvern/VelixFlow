#include "VelixFlow/RenderAPI/Renderer.hpp"
#include "VelixFlow/Logger.hpp"
#include <glm/gtc/matrix_transform.hpp>

ELIX_NAMESPACE_BEGIN
namespace render
{
    // std::shared_ptr<FrameBuffer> Renderer::initFbo(int width, int height)
    // {
    //     m_fbo = std::make_shared<FrameBuffer>();

    //     m_fbo->addAttachment(FrameBuffer::Attachment::DEPTH_STENCIL);
    //     m_fbo->create(width, height, FrameBuffer::InternalFormat::DEPTH24_STENCIL8, true);

    //     return m_fbo;
    // }
    
    // std::shared_ptr<FrameBuffer> Renderer::getFbo() const
    // {
    //     return m_fbo;
    // }

    void Renderer::renderAll(const FrameData& frameData, Scene* scene)
    {
        for(auto& renderPath : m_renderPasses)
        {
            if(!renderPath->shouldExecute())
                continue;

            renderPath->render(frameData, scene);
        }
    }

    const FrameData& Renderer::updateFrameData(components::CameraComponent* camera, int windowWidth, int windowHeight)
    {
        if (!camera)
            return m_frameData;

        m_frameData.projectionMatrix = camera->getProjectionMatrix();
        m_frameData.viewMatrix = camera->getViewMatrix();
        m_frameData.cameraPosition = camera->getPosition();

        m_frameData.uiProjection = glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f);
        m_frameData.flippedUiProjection = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight);
        m_frameData.screenPosition = {windowWidth, windowHeight};

        return m_frameData;
    }

    void Renderer::sortRenderPassesByPriority()
    {
        std::sort(m_renderPasses.begin(), m_renderPasses.end(), [](const auto& a, const auto& b) {return a->getPriority() < b->getPriority();});
    }

    void Renderer::removeRenderPath(const std::string& name)
    {
    m_renderPasses.erase(std::remove_if(m_renderPasses.begin(), m_renderPasses.end(), [&name](const auto& renderPath) { return renderPath->getName() == name; }), m_renderPasses.end());
    }

    void Renderer::renderPath(const std::string& name, const FrameData& frameData, Scene* scene)
    {
        for(const auto& path : m_renderPasses)
        {
            if(path->getName() == name)
                return path->render(frameData, scene);
            else
                ELIX_LOG_WARN("Could not find path with ", name, " name");
        }
    }

    void Renderer::renderPath(int priority, const FrameData& frameData, Scene* scene)
    {
        for(const auto& path : m_renderPasses)
        {
            if(path->getPriority() == priority)
                return path->render(frameData, scene);
            else
                ELIX_LOG_INFO("Could not find path with ", priority, " priority");
        }
    }
}

ELIX_NAMESPACE_END