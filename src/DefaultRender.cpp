#include <glad/glad.h>

#include "DefaultRender.hpp"
#include "ShaderManager.hpp"
#include "LightManager.hpp"
#include "MeshComponent.hpp"
#include "ParticleComponent.hpp"

window::ClearFlag elix::DefaultRender::getClearFlag()
{
    return window::ClearFlag::COLOR_BUFFER_BIT | window::ClearFlag::DEPTH_BUFFER_BIT | window::ClearFlag::STENCIL_BUFFER_BIT;
}

void elix::DefaultRender::render(const elix::FrameData& frameData, Scene* scene)
{
     if(!renderTarget_ && !m_window)
        return;

    if(!scene)
        return;

    if(renderTarget_)
        renderTarget_->bind();

    renderTarget_ ? window::Window::setViewport(0, 0, renderTarget_->getWidth(), renderTarget_->getHeight()) : m_window->viewport();

    glEnable(GL_DEPTH_TEST);

    window::Window::clear(getClearFlag());

    const auto& gameObjects = scene->getGameObjects();

    elix::Shader *skeletonShader = ShaderManager::instance().getShader(ShaderManager::ShaderType::SKELETON);
    elix::Shader *staticShader = ShaderManager::instance().getShader(ShaderManager::ShaderType::STATIC);
    
    staticShader->bind();
    staticShader->setMat4("view", frameData.viewMatrix);
    staticShader->setMat4("projection", frameData.projectionMatrix);
    staticShader->setVec3("viewPos", frameData.cameraPosition);
    LightManager::instance().sendLightsIntoShader(*staticShader);

    staticShader->unbind();

    skeletonShader->bind();
    skeletonShader->setMat4("view", frameData.viewMatrix);
    skeletonShader->setMat4("projection", frameData.projectionMatrix);
    skeletonShader->setVec3("viewPos", frameData.cameraPosition);
    skeletonShader->unbind();

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    for (const auto &gameObject : gameObjects)
    {
        bool isSelected = (gameObject.get() == m_selectedGameObject);

        if (gameObject->hasComponent<MeshComponent>())
        {
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(isSelected ? 0xFF : 0x00);

            auto *justShader = gameObject->getComponent<MeshComponent>()->getModel()->hasSkeleton() ? skeletonShader : staticShader;

            justShader->bind();
            justShader->setMat4("model", gameObject->getTransformMatrix());

            if (justShader == skeletonShader)
            {
                const std::vector<glm::mat4> &boneMatrices = gameObject->getComponent<MeshComponent>()->getModel()->getSkeleton()->getFinalMatrices();
                justShader->setMat4Array("finalBonesMatrices", boneMatrices);
            }

            gameObject->getComponent<MeshComponent>()->render(true);
            justShader->unbind();
        }
    }

    for(const auto& gameObject : scene->getGameObjects())
    {
        if(gameObject->hasComponent<ParticleComponent>())
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(GL_FALSE);

            auto s = ShaderManager::instance().getShader(ShaderManager::ShaderType::PARTICLE);

            s->bind();

            s->setMat4("uViewProj", frameData.projectionMatrix);

            gameObject->getComponent<ParticleComponent>()->render();

            glDepthMask(GL_TRUE);
        }
    }

    if (scene->getSkybox())
        scene->getSkybox()->render(frameData.viewMatrix, frameData.projectionMatrix);

    for (const auto &drawable : scene->getDrawables())
        drawable->draw();


    if(renderTarget_)
        renderTarget_->unbind();

    glDisable(GL_DEPTH_TEST);
}

bool elix::DefaultRender::shouldExecute() const
{
    return true;
}

std::string elix::DefaultRender::getName() const
{
    return "DefaultRender";
}

int elix::DefaultRender::getPriority() const
{
    return 1;
}
