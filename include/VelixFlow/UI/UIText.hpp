#ifndef TEXT_HPP
#define TEXT_HPP

#include "ft2build.h"
#include FT_FREETYPE_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <unordered_map>


#include <string>

#include "UIElement.hpp"
#include "VelixFlow/DefaultMacros.hpp"

ELIX_NAMESPACE_BEGIN

namespace ui
{
    class UIText : public UIElement
    {
    public:
        UIText();

        void setFont(const std::string& pathToFont);
        void setText(const std::string& text);

        void draw(const glm::mat4& projection, const glm::mat4& flippedProjection) override;

        void setScale(float scale);

        ~UIText();

    private:

        struct Character
        {
            unsigned int TextureID;
            glm::ivec2   Size;
            glm::ivec2   Bearing;
            unsigned int Advance;
        };

        std::unordered_map<char, Character> m_characters;

        FT_Library m_ftLibrary;

        std::string m_fontPath;

        std::string m_text;

        float m_scale{1.0f};

        unsigned int m_vao;
        unsigned int m_vbo;
    };

} //namespace ui

ELIX_NAMESPACE_END

#endif //TEXT_HPP
