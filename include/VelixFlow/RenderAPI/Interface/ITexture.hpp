#ifndef ITEXTURE_HPP
#define ITEXTURE_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/TextureParameters.hpp"

#include <string>
#include <cstdint>

ELIX_NAMESPACE_BEGIN

namespace texture
{
    class ITexture
    {
    public:
        virtual void bind(uint32_t slot = 0) const = 0;
        virtual void unbind(uint32_t slot = 0) const = 0;

        virtual bool load(const std::string& path) = 0;

        virtual void upload() = 0;
        virtual bool isUploaded() const = 0;

        void setTextureUsage(TextureUsage usage)
        {
            textureUsage_ = usage;
        }

        TextureUsage getTextureUsage() const
        {
            return textureUsage_;
        }

        virtual void addDefaultParameters() = 0;
        virtual void addParameter(ParameterType type, ParameterValue value) = 0;

        virtual ~ITexture() = default;

    protected:
        TextureUsage textureUsage_{TextureUsage::Standard2D};
    };
}

ELIX_NAMESPACE_END

#endif //ITEXTURE_HPP