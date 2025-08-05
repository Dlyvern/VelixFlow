#include <glad/glad.h>

#include "VelixGL/GLSceneRender.hpp"
#include "VelixGL/ShaderManager.hpp"
#include "VelixFlow/Components/MeshComponent.hpp"
// #include "VelixFlow/Components/ParticleComponent.hpp"
#include "VelixGL/DrawCall.hpp"
#include "VelixFlow/Components/TransformComponent.hpp"

#include "VelixGL/GLMesh.hpp"

ELIX_NAMESPACE_BEGIN

namespace render
{
    void GLSceneRender::render(const FrameData& frameData, Scene* scene)
    {
        // if(!renderTarget_ && !m_window)
            // return;

        if(!m_window)
            return;

        if(!scene)
            return;

        // if(renderTarget_)
        //     renderTarget_->bind();

        //Get window width and height
        // renderTarget_ ? glViewport(0, 0, renderTarget_->getWidth(), renderTarget_->getHeight()) : m_window->viewport();

        glEnable(GL_DEPTH_TEST);

        window::ClearFlag flags = getClearFlag();

        GLbitfield mask = 0;
        if (flags & window::ClearFlag::COLOR_BUFFER_BIT)   mask |= GL_COLOR_BUFFER_BIT;
        if (flags & window::ClearFlag::DEPTH_BUFFER_BIT)   mask |= GL_DEPTH_BUFFER_BIT;
        if (flags & window::ClearFlag::STENCIL_BUFFER_BIT) mask |= GL_STENCIL_BUFFER_BIT;

        glClear(mask);

        const auto& gameObjects = scene->getGameObjects();

        auto* skeletonShader = ShaderManager::instance().getShader(ShaderManager::ShaderType::SKELETON);
        auto* staticShader = ShaderManager::instance().getShader(ShaderManager::ShaderType::STATIC);
        
        staticShader->bind();
        staticShader->setMat4("view", frameData.viewMatrix);
        staticShader->setMat4("projection", frameData.projectionMatrix);
        staticShader->setVec3("viewPos", frameData.cameraPosition);

        for (size_t i = 0; i < scene->getLights().size(); ++i)
        {
            const auto light = scene->getLights().at(i);
            staticShader->setInt("lights[" + std::to_string(i) + "].type", static_cast<int>(light->type));
            staticShader->setVec3("lights[" + std::to_string(i) + "].position", light->position);
            staticShader->setVec3("lights[" + std::to_string(i) + "].color", light->color);
            staticShader->setFloat("lights[" + std::to_string(i) + "].strength", light->strength);
            staticShader->setFloat("lights[" + std::to_string(i) + "].radius", light->radius);
            staticShader->setVec3("lights[" + std::to_string(i) + "].direction", light->direction);
            staticShader->setFloat("lights[" + std::to_string(i) + "].cutoff", light->cutoff);
            staticShader->setFloat("lights[" + std::to_string(i) + "].outerCutoff", light->outerCutoff);
        }

        staticShader->unbind();

        skeletonShader->bind();
        skeletonShader->setMat4("view", frameData.viewMatrix);
        skeletonShader->setMat4("projection", frameData.projectionMatrix);
        skeletonShader->setVec3("viewPos", frameData.cameraPosition);
        skeletonShader->unbind();

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);

        for (const auto& gameObject : gameObjects)
        {
            const bool isSelected = (gameObject.get() == m_selectedGameObject);

            if (auto meshComponent = gameObject->getComponent<components::MeshComponent>())
            {
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilMask(isSelected ? 0xFF : 0x00);

                auto model = meshComponent->getModel();
                const bool isSkeleton = model->hasSkeleton();

                auto justShader = isSkeleton ? skeletonShader : staticShader;

                justShader->bind();
                justShader->setMat4("model", gameObject->getComponent<components::TransformComponent>()->getTransformMatrix());

                if (isSkeleton)
                {
                    const std::vector<glm::mat4> &boneMatrices = model->getSkeleton()->getFinalMatrices();
                    justShader->setMat4Array("finalBonesMatrices", boneMatrices);
                }

                for (int meshIndex = 0; meshIndex < model->getNumMeshes(); meshIndex++)
                {
                    auto imesh = model->getMesh(meshIndex);

                    auto mesh = dynamic_cast<GLMesh*>(imesh.get());

                    Material* material = meshComponent->getMaterialOverride(meshIndex);
                    
                    if (!material)
                        continue;

                    material->bind(justShader);

                    const auto& vertexArray = mesh->getVertexArray();
                    vertexArray.bind();
                    DrawCall::draw(DrawCall::DrawMode::TRIANGLES, mesh->getIndices().size(), DrawCall::DrawType::UNSIGNED_INT, nullptr);
                    vertexArray.unbind();
                }

                justShader->unbind();
            }

            // if(gameObject->hasComponent<components::ParticleComponent>())
            // {
            //     glEnable(GL_BLEND);
            //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            //     glDepthMask(GL_FALSE);

            //     auto s = ShaderManager::instance().getShader(ShaderManager::ShaderType::PARTICLE);

            //     s->bind();

            //     s->setMat4("uViewProj", frameData.projectionMatrix);

            //     gameObject->getComponent<components::ParticleComponent>()->render();

            //     s->unbind();

            //     glDepthMask(GL_TRUE);
            // }
        }

        // if (scene->getSkybox())
        //     scene->getSkybox()->render(frameData.viewMatrix, frameData.projectionMatrix);

        // if(renderTarget_)
        //     renderTarget_->unbind();

        glDisable(GL_DEPTH_TEST);

    }

    int GLSceneRender::getPriority() const
    {
        return 1;
    }

    window::ClearFlag GLSceneRender::getClearFlag()
    {
        return window::ClearFlag::COLOR_BUFFER_BIT | window::ClearFlag::DEPTH_BUFFER_BIT | window::ClearFlag::STENCIL_BUFFER_BIT;
    }

    void GLSceneRender::setSelectedGameObject(GameObject* gameObject)
    {
        m_selectedGameObject = gameObject;
    }

    void GLSceneRender::setWindow(window::Window* window)
    {
        m_window = window;
    }

    bool GLSceneRender::shouldExecute() const
    {
        return true;
    }

    std::string GLSceneRender::getName() const
    {
        return "GLSceneRender";
    }
}

ELIX_NAMESPACE_END