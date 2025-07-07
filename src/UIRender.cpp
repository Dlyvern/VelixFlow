#include <glad/glad.h>
#include "UIRender.hpp"
#include "ShaderManager.hpp"
#include "Logger.hpp"

ELIX_NAMESPACE_BEGIN

UIRender::UIRender()
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
}

window::ClearFlag UIRender::getClearFlag()
{
    return window::ClearFlag::NONE;
}

bool UIRender::shouldExecute() const
{
    return true;
}

std::string UIRender::getName() const
{
    return "UIRender";
}

void UIRender::render(const elix::FrameData& frameData, Scene* scene)
{
    if(!scene)
        return;

    auto shader = ShaderManager::instance().getShader(ShaderManager::ShaderType::UI);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const auto& uiElements = scene->getUIElements();

    shader->bind();

    for(const auto& uiElement : uiElements)
    {
        if(!uiElement->isVisible())
            continue;

        shader->setMat4("uProjection", frameData.projectionMatrix);
        shader->setVec2("uPosition", uiElement->getPosition());
        shader->setVec2("uSize", uiElement->getSize());
        shader->setVec4("uColor", uiElement->getColor());
        shader->setFloat("uAlpha", uiElement->getAlpha());

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    shader->unbind();

    glEnable(GL_DEPTH_TEST);
}

int UIRender::getPriority() const
{
    return 10;
}


ELIX_NAMESPACE_END