#ifndef VK_TEXTURE_HPP
#define VK_TEXTURE_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/Interface/ITexture.hpp"
#include "VelixFlow/Image.hpp"
#include <cstdint>

ELIX_NAMESPACE_BEGIN

namespace texture
{
    class VKTexture : public ITexture
    {
    public:
        void bind(uint32_t slot = 0) const override;
        void unbind(uint32_t slot = 0) const override;

        bool load(const std::string& path) override;

        void upload() override;
        bool isUploaded() const override;

        void addDefaultParameters() override;
        void addParameter(ParameterType type, ParameterValue value) override;
    private:
        Image m_image;
    };
} //namespace texture

ELIX_NAMESPACE_END

#endif //VK_TEXTURE_HPP