#include "TexturesManager.hpp"

#include <glad/glad.h>

TexturesManager::TexturesManager()
{
}

TexturesManager& TexturesManager::instance()
{
    static TexturesManager instance;
    return instance;
}

void TexturesManager::reset()
{
    m_usedUnits.clear();
    m_nextUnit = 0;
}

int TexturesManager::bindTexture(unsigned int textureId, unsigned int target)
{
    // GL_TEXTURE_BINDING
    //  glGetIntegerv(GL_FRAMEBUFFER_BINDING, reinterpret_cast<GLint*>(&previousFBO));
    //  std::cout << previousFBO << std::endl;

    GLenum unitEnum = GL_TEXTURE0 + m_nextUnit;
    glActiveTexture(unitEnum);
    glBindTexture(target, textureId);
    m_usedUnits.emplace_back(unitEnum, target);
    return m_nextUnit++;
}
