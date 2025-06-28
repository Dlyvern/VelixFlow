#include "Renderer.hpp"
#include "Logger.hpp"


std::shared_ptr<elix::FrameBuffer> elix::Renderer::initFbo(int width, int height)
{
    m_fbo = std::make_shared<elix::FrameBuffer>();

    m_fbo->addAttachment(elix::FrameBuffer::Attachment::DEPTH_STENCIL);
    m_fbo->create(width, height, elix::FrameBuffer::InternalFormat::DEPTH24_STENCIL8, true);

    return m_fbo;
}
 
std::shared_ptr<elix::FrameBuffer> elix::Renderer::getFbo() const
{
    return m_fbo;
}

void elix::Renderer::renderAll(const elix::FrameData& frameData, Scene* scene)
{
    for(auto& renderPath : m_renderPasses)
    {
       if(!renderPath->shouldExecute())
           continue;

       renderPath->render(frameData, scene);
    }
}

const elix::FrameData& elix::Renderer::updateFrameData(elix::CameraComponent* camera)
{
    if (!camera)
        return m_frameData;

    m_frameData.projectionMatrix = camera->getProjectionMatrix();
    m_frameData.viewMatrix = camera->getViewMatrix();
    m_frameData.cameraPosition = camera->getPosition();

    return m_frameData;
}

void elix::Renderer::sortRenderPassesByPriority()
{
    std::sort(m_renderPasses.begin(), m_renderPasses.end(), [](const auto& a, const auto& b) {return a->getPriority() < b->getPriority();});
}

void elix::Renderer::removeRenderPath(const std::string& name)
{
 m_renderPasses.erase(std::remove_if(m_renderPasses.begin(), m_renderPasses.end(), [&name](const auto& renderPath) { return renderPath->getName() == name; }), m_renderPasses.end());
}

void elix::Renderer::renderPath(const std::string& name, const FrameData& frameData, Scene* scene)
{
    for(const auto& path : m_renderPasses)
    {
        if(path->getName() == name)
            return path->render(frameData, scene);
        else
            ELIX_LOG_WARN("Could not find path with ", name, " name");
    }
}


void elix::Renderer::renderPath(int priority, const FrameData& frameData, Scene* scene)
{
    for(const auto& path : m_renderPasses)
    {
        if(path->getPriority() == priority)
            return path->render(frameData, scene);
        else
            ELIX_LOG_INFO("Could not find path with ", priority, " priority");
    }
}
