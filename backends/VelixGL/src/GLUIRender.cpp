#include <glad/glad.h>
#include "VelixGL/GLUIRender.hpp"
#include "VelixGL/ShaderManager.hpp"
#include "VelixFlow/Logger.hpp"
#include "VelixFlow/UI/UIText.hpp"
#include "VelixFlow/Scene.hpp"

#include <iostream>

#include <glm/ext/matrix_clip_space.hpp>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>

#include "VelixGL/VertexArray.hpp"
#include "VelixGL/GLBuffer.hpp"

#include "VelixGL/DrawCall.hpp"

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

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);


        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_fontCache = std::make_shared<GLFontCache>();
    }

    bool GLUIRender::shouldExecute() const
    {
        return true;
    }

    std::string GLUIRender::getName() const
    {
        return "GLUIRender";
    }

    void GLUIRender::drawUIElements(const FrameData& frameData, elix::ui::UIWidget* element)
    {
        if (!element || !element->isVisible() || dynamic_cast<ui::UIText*>(element))
            return;

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

    void GLUIRender::drawTextElements(const FrameData& frameData, elix::ui::UIWidget* element)
    {
        if (!element || !element->isVisible())
            return;

        // ELIX_LOG_INFO("Drawing ", element->getName(), " at ", element->getPosition().x, " ",  element->getPosition().y, " size: ", element->getSize().x, " ", element->getSize().y);

        if (auto* text = dynamic_cast<ui::UIText*>(element))
        {
            if (!text->getFont()) return;
            
            auto position = text->getPosition();
            const auto& scale = text->getScale();

            auto shader = ShaderManager::instance().getShader(ShaderManager::TEXT);

            shader->bind();

            shader->setVec3("textColor", glm::vec3{1.0f});
            shader->setMat4("projection", frameData.flippedUiProjection);
            shader->setInt("text", 0);

            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(m_vao);

            for (char c : text->getText())
            {
                const ui::Glyph* glyph = text->getFont()->getGlyph(c);

                if (!glyph)
                {
                    ELIX_LOG_WARN("Failed to get glyph for ", c, " character");
                    continue;
                }

                GLuint texture = m_fontCache->getGlyphTexture(glyph);

                if (texture == 0)
                {
                    ELIX_LOG_WARN("Failed to get texture for ", c, " character");
                    continue;
                }

                float xpos = position.x + glyph->bearing.x * scale;
                float ypos = position.y - (glyph->size.y - glyph->bearing.y) * scale;
                float w = glyph->size.x * scale;
                float h = glyph->size.y * scale;

                float vertices[6][4] = {
                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos,     ypos,       0.0f, 1.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },

                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },
                    { xpos + w, ypos + h,   1.0f, 0.0f }
                };

                glBindTexture(GL_TEXTURE_2D, texture);
                glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                glDrawArrays(GL_TRIANGLES, 0, 6);

                position.x += (glyph->advance >> 6) * scale;
            }
            
            shader->unbind();

            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        for (const auto& child : element->getChildren())
            drawTextElements(frameData, child.get());
    }

    void GLUIRender::render(const FrameData& frameData, Scene* scene)
    {
        if(!scene)
            return;

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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