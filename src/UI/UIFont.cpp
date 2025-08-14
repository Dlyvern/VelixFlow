#include "VelixFlow/UI/UIFont.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "VelixFlow/Logger.hpp"

ELIX_NAMESPACE_BEGIN

namespace ui
{
    const std::string& UIFont::getFontPath() const
    {
        return m_pathToFont;
    }

    glm::vec2 UIFont::calculateTextSize(const std::string& text, float scale) const
    {
        glm::vec2 size(0.0f);

        for (char c : text)
        {
            auto it = m_glyphs.find(c);
            
            if (it != m_glyphs.end())
            {
                const Glyph& g = it->second;
                size.x += (g.advance >> 6) * scale;
                size.y = std::max(size.y, g.size.y * scale);
            }
        }

        return size;
    }


    bool UIFont::load(const std::string& path)
    {
        FT_Library ft;

        if (FT_Init_FreeType(&ft)) 
        {
            ELIX_LOG_ERROR("Failed to initialize FreeType");
            return false;
        }
        
        FT_Face face;

        if (FT_New_Face(ft, path.c_str(), 0, &face))
        {
            FT_Done_FreeType(ft);
            ELIX_LOG_ERROR("Failed to load font");
            
            return false;
        }

        FT_Set_Pixel_Sizes(face, 0, 48);
        
        try
        {
            for (unsigned char c = 32; c < 127; c++) 
            {
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    ELIX_LOG_WARN("Failed to load glyph '", c, "'");
                    continue;
                }

                Glyph glyph;
                glyph.size = {face->glyph->bitmap.width, face->glyph->bitmap.rows};
                glyph.bearing = {face->glyph->bitmap_left, face->glyph->bitmap_top};
                glyph.advance = face->glyph->advance.x;
                
                glyph.bitmapWidth = face->glyph->bitmap.width;
                glyph.bitmapRows = face->glyph->bitmap.rows;

                glyph.bitmapData.assign(
                    face->glyph->bitmap.buffer,
                    face->glyph->bitmap.buffer + face->glyph->bitmap.width * face->glyph->bitmap.rows
                );

                m_glyphs.insert(std::make_pair(c, std::move(glyph)));
            }
        } catch (...) 
        {
            FT_Done_Face(face);
            FT_Done_FreeType(ft);
            return false;
        }

        if(m_glyphs.empty())
        {
            FT_Done_FreeType(ft);
            ELIX_LOG_ERROR("Failed to load font");
            
            return false;
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        m_pathToFont = path;

        return true;
    }

    const Glyph* UIFont::getGlyph(char c) const
    {
        auto it = m_glyphs.find(c);
        return it != m_glyphs.end() ? &it->second : nullptr;
    }


} //namespace ui


ELIX_NAMESPACE_END