#include "VelixFlow/UI/UIText.hpp"
#include <iostream>

// #include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
// #include "VelixFlow/ShaderManager.hpp"

ELIX_NAMESPACE_BEGIN

ui::UIText::UIText()
{
    if (FT_Init_FreeType(&m_ftLibrary))
    {
        //Are we stupid here?
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
    }
}

void ui::UIText::setScale(float scale)
{
    m_scale = scale;
}

void ui::UIText::draw(const glm::mat4& projection, const glm::mat4& flippedProjection)
{
    if (m_vao == 0 || m_vbo == 0)
    {
        std::cout << "Text::draw(): VAO/VBO not initialized." << std::endl;
        return;
    }

    // const auto shader = ShaderManager::instance().getShader(ShaderManager::ShaderType::TEXT);

    // shader->bind();

    float x = getPosition().x;
    float y = getPosition().y;

    // shader->setVec3("textColor", glm::vec3{1.0f});
    // shader->setMat4("projection", flippedProjection);
    // shader->setInt("text", 0);
    
    // glActiveTexture(GL_TEXTURE0);
    // glBindVertexArray(m_vao);

    // for (char c : m_text)
    // {
    //     auto it = m_characters.find(c);
        
    //     if (it == m_characters.end())
    //         continue;

    //     Character& ch = it->second;

    //     float xPosition = x + ch.Bearing.x * m_scale;
    //     float yPosition = y - (ch.Size.y - ch.Bearing.y) * m_scale;

    //     float w = ch.Size.x * m_scale;
    //     float h = ch.Size.y * m_scale;

    //     float vertices[6][4] =
    //     {
    //         { xPosition,     yPosition + h,   0.0f, 0.0f },
    //         { xPosition,     yPosition,       0.0f, 1.0f },
    //         { xPosition + w, yPosition,       1.0f, 1.0f },

    //         { xPosition,     yPosition + h,   0.0f, 0.0f },
    //         { xPosition + w, yPosition,       1.0f, 1.0f },
    //         { xPosition + w, yPosition + h,   1.0f, 0.0f }
    //     };

    //     glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    //     glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    //     glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    //     glBindBuffer(GL_ARRAY_BUFFER, 0);
    //     glDrawArrays(GL_TRIANGLES, 0, 6);

    //     x += (ch.Advance >> 6) * m_scale;
    // }

    // glBindVertexArray(0);
    // glBindTexture(GL_TEXTURE_2D, 0);
}

void ui::UIText::setFont(const std::string& pathToFont)
{
    // if (pathToFont.empty())
    // {
    //     std::cout << "Text::setFont(): Failed to load font, the given path is empty" << std::endl;
    //     return;
    // }

    // FT_Face face;

    // if (FT_New_Face(m_ftLibrary, pathToFont.c_str(), 0, &face))
    // {
    //     std::cout << "Text::setFont(): Failed to load font for some reason" << std::endl;
    //     return;
    // }

    // FT_Set_Pixel_Sizes(face, 0, 48);

    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // for (unsigned char c = 0; c < 128; c++)
    // {
    //     if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    //     {
    //         std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
    //         continue;
    //     }

    //     unsigned int texture;
    //     glGenTextures(1, &texture);
    //     glBindTexture(GL_TEXTURE_2D, texture);
    //     glTexImage2D(
    //         GL_TEXTURE_2D,
    //         0,
    //         GL_RED,
    //         face->glyph->bitmap.width,
    //         face->glyph->bitmap.rows,
    //         0,
    //         GL_RED,
    //         GL_UNSIGNED_BYTE,
    //         face->glyph->bitmap.buffer
    //     );
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //     Character character = {
    //         texture,
    //         glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
    //         glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
    //         static_cast<unsigned int>(face->glyph->advance.x)
    //     };

    //     m_characters.insert(std::pair<char, Character>(c, character));
    // }
    // glBindTexture(GL_TEXTURE_2D, 0);

    // FT_Done_Face(face);
    // FT_Done_FreeType(m_ftLibrary);

    // glGenVertexArrays(1, &m_vao);
    // glGenBuffers(1, &m_vbo);
    // glBindVertexArray(m_vao);
    // glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);
}

void ui::UIText::setText(const std::string &text)
{
    m_text = text;
}

ui::UIText::~UIText()
{
    // for (auto& [_, ch] : m_characters)
    //     glDeleteTextures(1, &ch.TextureID);

    // if (m_vbo) glDeleteBuffers(1, &m_vbo);
    // if (m_vao) glDeleteVertexArrays(1, &m_vao);

    // if (m_ftLibrary)
    //     FT_Done_FreeType(m_ftLibrary);
}

ELIX_NAMESPACE_END