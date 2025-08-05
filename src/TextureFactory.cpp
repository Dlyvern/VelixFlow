#include "VelixFlow/TextureFactory.hpp"

ELIX_NAMESPACE_BEGIN

namespace texture
{
    void TextureFactory::init(IRenderContext* renderContext)
    {
        if(s_instance)
        {
            s_instance.release();
            s_instance = nullptr;
        }

        s_instance = std::make_unique<TextureFactory>();

        s_instance->m_renderContext = renderContext;
    }

    IRenderContext* TextureFactory::getRenderContext()
    {
        return m_renderContext;
    }

    std::unique_ptr<ITexture> TextureFactory::createTexture(const std::string& path)
    {
        if(!s_instance)
            return nullptr;
        return s_instance->m_renderContext->createTexture(path);
    }

} //namespace texture

ELIX_NAMESPACE_END