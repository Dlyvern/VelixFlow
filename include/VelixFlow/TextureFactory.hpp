#ifndef TEXTURE_FACTORY_HPP
#define TEXTURE_FACTORY_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/Interface/IRenderContext.hpp"
#include "VelixFlow/RenderAPI/Interface/ITexture.hpp"
#include <memory>
#include <string>

ELIX_NAMESPACE_BEGIN

namespace texture
{
    class TextureFactory
    {
    public:
        static void init(IRenderContext* renderContext);
        static std::unique_ptr<ITexture> createTexture(const std::string& path);
        IRenderContext* getRenderContext();
    private:
        static inline std::unique_ptr<TextureFactory> s_instance;
        IRenderContext* m_renderContext;
    };
} //namespace texture

ELIX_NAMESPACE_END

#endif //TEXTURE_FACTORY_HPP