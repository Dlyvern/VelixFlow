#include "VelixGL/GLShadowRender.hpp"
#include "VelixGL/ShaderManager.hpp"
#include "VelixFlow/Components/MeshComponent.hpp"
#include "VelixFlow/Components/TransformComponent.hpp"
#include "VelixGL/DrawCall.hpp"
#include "VelixFlow/Scene.hpp"

#include "VelixGL/GLMesh.hpp"

ELIX_NAMESPACE_BEGIN

namespace render
{
    GLShadowRender::GLShadowRender(const std::vector<std::shared_ptr<lighting::Light>>& lights)
    {
        m_shadowSystem.init(lights);
    }

    void GLShadowRender::render(const FrameData& frameData, Scene* scene)
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
                if (auto meshComponent = gameObject->getComponent<components::MeshComponent>())
                {
                    const auto model = meshComponent->getModel();
                    const bool isSkeleton = model->hasSkeleton();
                    const auto justShader = isSkeleton ? skeletonShadowShader : staticShadowShader;

                    justShader->bind();
                    justShader->setMat4("model", gameObject->getComponent<components::TransformComponent>()->getTransformMatrix());
                    justShader->setMat4("lightSpaceMatrix", lightMatrix);

                    if (isSkeleton)
                    {
                        const std::vector<glm::mat4> &boneMatrices = model->getSkeleton()->getFinalMatrices();
                        justShader->setMat4Array("finalBonesMatrices", boneMatrices);
                    }

                    for (int meshIndex = 0; meshIndex < model->getNumMeshes(); meshIndex++)
                    {
                        auto imesh = model->getMesh(meshIndex);

                        auto mesh = dynamic_cast<GLMesh*>(imesh.get());

                        const auto& vertexArray = mesh->getVertexArray();
                        vertexArray.bind();
                        DrawCall::draw(DrawCall::DrawMode::TRIANGLES, mesh->getIndices().size(), DrawCall::DrawType::UNSIGNED_INT, nullptr);
                        vertexArray.unbind();
                    }

                    justShader->unbind();
                }
            }

            m_shadowSystem.endShadowPass();
        }

        //TODO Default render shit, it should not be here

        auto* skeletonShader = ShaderManager::instance().getShader(ShaderManager::ShaderType::SKELETON);
        auto* staticShader = ShaderManager::instance().getShader(ShaderManager::ShaderType::STATIC);
    
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

    bool GLShadowRender::shouldExecute() const
    {
        return true;
    }

    std::string GLShadowRender::getName() const
    {
        return "GLShadowRender";
    }

    int GLShadowRender::getPriority() const
    {
        return 0;
    }
}
ELIX_NAMESPACE_END