#ifndef GL_TEXTURE_HPP
#define GL_TEXTURE_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/Interface/ITexture.hpp"
#include "VelixFlow/Image.hpp"

#include <vector>

ELIX_NAMESPACE_BEGIN

namespace texture
{
    class GLTexture : public ITexture
    {
    public:
        explicit GLTexture(const std::string& name);
        GLTexture() = default;

        const std::string& getName() const;

        void bind(uint32_t slot = 0) const override;
        void unbind(uint32_t slot = 0) const override;

        bool load(const std::string& path) override;

        void setImage(const Image& image);

        unsigned int getId() const;

        void upload() override;
        bool isUploaded() const override;

        unsigned int& getId();

        void addDefaultParameters() override;
        void addParameter(ParameterType type, ParameterValue value) override;

        ~GLTexture() override;
    private:
        std::vector<texture::Parameter> m_textureParameters;
        bool m_isBaked{false};
        unsigned int m_id{0};
        Image m_image;
        std::string m_name;
    };
} //namespace texture

ELIX_NAMESPACE_END

#endif //GL_TEXTURE_HPP