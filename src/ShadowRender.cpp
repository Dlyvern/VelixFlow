#include "ShadowRender.hpp"
#include "ShaderManager.hpp"
#include "MeshComponent.hpp"

elix::ShadowRender::ShadowRender(const std::vector<std::shared_ptr<lighting::Light>>& lights)
{
    m_shadowSystem.init(lights);
}

void elix::ShadowRender::render(const elix::FrameData& frameData, Scene* scene)
{
    if(!scene)
        return;

    const auto& gameObjects = scene->getGameObjects();

    const auto *staticShadowShader = ShaderManager::instance().getShader(ShaderManager::ShaderType::STATIC_SHADOW);
    const auto *skeletonShadowShader = ShaderManager::instance().getShader(ShaderManager::ShaderType::SKELETON_SHADOW);

    for (const auto &light : scene->getLights())
    {
        m_shadowSystem.updateLightMatrix(light.get());

        m_shadowSystem.beginShadowPass(light.get());

        glm::mat4 lightMatrix = m_shadowSystem.getLightMatrix(light.get());

        for (const auto &gameObject : gameObjects)
        {
            const auto justShader = gameObject->getComponent<MeshComponent>()->getModel()->hasSkeleton() ? skeletonShadowShader : staticShadowShader;

            justShader->bind();
            justShader->setMat4("model", gameObject->getTransformMatrix());
            justShader->setMat4("lightSpaceMatrix", lightMatrix);

            if (justShader == skeletonShadowShader)
            {
                const std::vector<glm::mat4> &boneMatrices = gameObject->getComponent<MeshComponent>()->getModel()->getSkeleton()->getFinalMatrices();
                justShader->setMat4Array("finalBonesMatrices", boneMatrices);
            }

            gameObject->getComponent<MeshComponent>()->render();
            justShader->unbind();
        }

        m_shadowSystem.endShadowPass();
    }

    //TODO Default render shit, it should not be here

    elix::Shader *skeletonShader = ShaderManager::instance().getShader(ShaderManager::ShaderType::SKELETON);
    elix::Shader *staticShader = ShaderManager::instance().getShader(ShaderManager::ShaderType::STATIC);
 
    const auto &lights = scene->getLights();

    staticShader->bind();

    //TODO remove hardcoded texture slot
    for (size_t index = 0; index < lights.size(); ++index)
    {
        int textureSlot = 20 + index;
        auto light = lights.at(index);
        const auto &lightMatrix = m_shadowSystem.getLightMatrix(light.get());
        staticShader->setInt("shadowMaps[" + std::to_string(index) + "]", textureSlot);
        staticShader->setMat4("lightSpaceMatrices[" + std::to_string(index) + "]", lightMatrix);
        m_shadowSystem.bindShadowPass(light.get(), textureSlot);
    }

    staticShader->unbind();
}

bool elix::ShadowRender::shouldExecute() const
{
    return true;
}

std::string elix::ShadowRender::getName() const
{
    return "ShadowRender";
}

int elix::ShadowRender::getPriority() const
{
    return 0;
}
