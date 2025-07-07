#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <vector>

namespace elix
{
    class Texture
    {
    public:

        enum class TextureFormat
        {
            RGB,
            RGBA,
            RED,
            SRGB,
            SRGBA,
            DepthComponent
        };

        enum class TextureType
        {
            Diffuse,
            Specular,
            Normal,
            Metallic,
            Roughness,
            AO,
            Emissive,
            Height,
            Glossiness,
            Opacity,
            Depth2D,
            Undefined
        };

        enum class TextureUsage
        {
            Standard2D,
            RenderTarget
        };


        struct TextureData
        {
        public:
            int width{0};
            int height{0};
            int numberOfChannels{0};
            unsigned char* data{nullptr};
            TextureFormat format;
            TextureFormat secondFormat;
        };

        enum class ParameterType
        {
            MIN_FILTER,
            MAG_FILTER,
            WRAP_S,
            WRAP_T 
        };

        enum class ParameterValue
        {
            LINEAR,
            REPEAT
        };

        struct Parameter
        {
            ParameterType type;
            ParameterValue value;
            TextureUsage usage;
        };

        explicit Texture(const std::string& filePath);

        Texture();

        static elix::Texture::TextureData loadImage(const std::string& filePath, bool flipVertically);

        void create();

        void addDefaultParameters();

        void addParameter(TextureUsage usage, ParameterType type, ParameterValue value);

        void load(const std::string& filePath);

        [[nodiscard]]const std::string& getName() const;

        [[nodiscard]]unsigned int getId() const;

        void bake();

        [[nodiscard]] bool isBaked() const;

        void bind(unsigned int slot = 0) const;

        void unbind(unsigned int slot) const;

        ~Texture();
    private:
        std::vector<Parameter> m_textureParameters;
        bool m_isBaked{false};
        unsigned int m_id{0};
        TextureData m_textureData;
        std::string m_name;
    };

} //namespace elix

#endif //TEXTURE_HPP
