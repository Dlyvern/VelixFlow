#include "VelixGL/GLFontCache.hpp"
#include "glad/glad.h"

ELIX_NAMESPACE_BEGIN

unsigned int GLFontCache::getGlyphTexture(const ui::Glyph* glyph)
{
    auto it = m_textureCache.find(glyph);

    if (it != m_textureCache.end())
        return it->second;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        glyph->bitmapWidth,
        glyph->bitmapRows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        glyph->bitmapData.data()
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_textureCache[glyph] = texture;

    return texture;
}

void GLFontCache::clear()
{
    for (auto& pair : m_textureCache)
        glDeleteTextures(1, &pair.second);
    
    m_textureCache.clear();
}

ELIX_NAMESPACE_END