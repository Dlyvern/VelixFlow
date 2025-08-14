#ifndef GL_FONT_CACHE_HPP
#define GL_FONT_CACHE_HPP

#include "VelixFlow/UI/UIFont.hpp"
#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/UI/UIFont.hpp"

#include <unordered_map>
#include <cstdint>

ELIX_NAMESPACE_BEGIN

class GLFontCache
{   
public:
    unsigned int getGlyphTexture(const ui::Glyph* glyph);
    void clear();

private:
    std::unordered_map<const ui::Glyph*, unsigned int> m_textureCache;
};

ELIX_NAMESPACE_END

#endif //GL_FONT_CACHE_HPP