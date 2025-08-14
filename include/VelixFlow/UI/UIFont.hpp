#ifndef UI_FONT_HPP
#define UI_FONT_HPP

#include "glm/vec2.hpp"

#include <string>
#include <unordered_map>
#include <vector>
#include "VelixFlow/DefaultMacros.hpp"

ELIX_NAMESPACE_BEGIN

namespace ui
{
    struct Glyph
    {
    public:
        unsigned int textureID;
        glm::ivec2 size;
        glm::ivec2 bearing;
        unsigned int advance;
        float bitmapWidth;
        float bitmapRows;
        std::vector<unsigned char> bitmapData;
    };

    class UIFont
    {
    public:
        bool load(const std::string& path);
        const Glyph* getGlyph(char c) const;
        const std::string& getFontPath() const;

        glm::vec2 calculateTextSize(const std::string& text, float scale) const;

    private:
        std::unordered_map<char, Glyph> m_glyphs;
        std::string m_pathToFont;
    };
    
} //namespace ui

ELIX_NAMESPACE_END

#endif //UI_FONT_HPP