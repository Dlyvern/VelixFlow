#include <glad/glad.h>
#include "VelixGL/GLUIRender.hpp"
#include "VelixGL/ShaderManager.hpp"
#include "VelixFlow/Logger.hpp"
// #include "VelixFlow/UI/UIText.hpp"
#include "VelixFlow/Scene.hpp"

ELIX_NAMESPACE_BEGIN

namespace render
{
    GLUIRender::GLUIRender()
    {   
        float quadVertices[] = 
        {
            0.0f, 1.0f,  0.0f, 1.0f,
            1.0f, 0.0f,  1.0f, 0.0f,
            0.0f, 0.0f,  0.0f, 0.0f,

            0.0f, 1.0f,  0.0f, 1.0f,
            1.0f, 1.0f,  1.0f, 1.0f,
            1.0f, 0.0f,  1.0f, 0.0f,
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        glBindVertexArray(0);
    }

    window::ClearFlag GLUIRender::getClearFlag()
    {
        return window::ClearFlag::COLOR_BUFFER_BIT | window::ClearFlag::DEPTH_BUFFER_BIT;
    }

    bool GLUIRender::shouldExecute() const
    {
        return true;
    }

    std::string GLUIRender::getName() const
    {
        return "GLUIRender";
    }

    void GLUIRender::drawUIElements(const FrameData& frameData, elix::ui::UIElement* element)
    {
        if (!element || !element->isVisible())
            return;

        // if (dynamic_cast<ui::UIText*>(element)) return;

        auto shader = ShaderManager::instance().getShader(ShaderManager::ShaderType::UI);

        // ELIX_LOG_INFO("Drawing ", element->getName(), " at ", element->getPosition().x, " ",  element->getPosition().y, " size: ", element->getSize().x, " ", element->getSize().y);

        shader->setMat4("uProjection", frameData.uiProjection);
        shader->setVec2("uPosition", element->getPosition());
        shader->setVec2("uSize", element->getSize());
        shader->setVec4("uColor", element->getColor());
        shader->setFloat("uAlpha", element->getAlpha());
        shader->setInt("uUseTexture", element->getTexture() ? 1 : 0);

        if (auto texture = element->getTexture())
        {
            if (!texture->isUploaded())
            {
                texture->addDefaultParameters();
                texture->upload();
            }

            texture->bind(0);
            shader->setInt("uTexture", 0);
        }
        else
            glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        for (const auto& child : element->getChildren())
            drawUIElements(frameData, child.get());
    }

    void GLUIRender::drawTextElements(const FrameData& frameData, elix::ui::UIElement* element)
    {
        if (!element || !element->isVisible())
            return;

        // ELIX_LOG_INFO("Drawing ", element->getName(), " at ", element->getPosition().x, " ",  element->getPosition().y, " size: ", element->getSize().x, " ", element->getSize().y);

        // if (auto* text = dynamic_cast<ui::UIText*>(element))
        //     text->draw(frameData.uiProjection, frameData.flippedUiProjection);

        for (const auto& child : element->getChildren())
            drawTextElements(frameData, child.get());
    }

    void GLUIRender::render(const FrameData& frameData, Scene* scene)
    {
        if(!scene)
            return;

        window::ClearFlag flags = getClearFlag();

        GLbitfield mask = 0;
        if (flags & window::ClearFlag::COLOR_BUFFER_BIT)   mask |= GL_COLOR_BUFFER_BIT;
        if (flags & window::ClearFlag::DEPTH_BUFFER_BIT)   mask |= GL_DEPTH_BUFFER_BIT;
        if (flags & window::ClearFlag::STENCIL_BUFFER_BIT) mask |= GL_STENCIL_BUFFER_BIT;

        glClear(mask);

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (const auto& element : scene->getUIElements())
            element->updateLayout(frameData.screenPosition);

        auto shader = ShaderManager::instance().getShader(ShaderManager::ShaderType::UI);

        shader->bind();
        for (const auto& element : scene->getUIElements())
            drawUIElements(frameData, element.get());
        shader->unbind();

        for (const auto& element : scene->getUIElements())
            drawTextElements(frameData, element.get());

        glEnable(GL_DEPTH_TEST);
    }

    int GLUIRender::getPriority() const
    {
        return 10;
    }
}
ELIX_NAMESPACE_END