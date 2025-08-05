#include "VelixVK/VKTexture.hpp"

ELIX_NAMESPACE_BEGIN

namespace texture
{
    void VKTexture::bind(uint32_t slot) const
    {

    }

    void VKTexture::unbind(uint32_t slot) const
    {

    }

    bool VKTexture::load(const std::string& path)
    {
        if(!m_image.load(path, false))
            return false;

        return true;
    }

    void VKTexture::upload()
    {

    }

    bool VKTexture::isUploaded() const
    {

    }

    void VKTexture::addDefaultParameters()
    {

    }

    void VKTexture::addParameter(ParameterType type, ParameterValue value)
    {

    }
} //namespace texture

ELIX_NAMESPACE_END