#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "VelixFlow/RenderAPI/TextureParameters.hpp"
#include "DefaultMacros.hpp"
#include <string>

ELIX_NAMESPACE_BEGIN

class Image
{
public:
    bool load(const std::string& pathToImage, bool flip = false);
    void free();

    int getWidth() const;
    int getHeight() const;
    int getNumberOfChannels() const;
    unsigned char* getData();
    float* getHdrData();
    bool isHdr() const;
    texture::TextureFormat getFirstFormat() const;
    texture::TextureFormat getSecondFormat() const;

    void setWidth(int width);
    void setHeight(int height);
    void setNumberOfChannels(int numberOfChannels);
    void setData(unsigned char* data);
    void setFirstFormat(texture::TextureFormat format);
    void setSecondFormat(texture::TextureFormat format);

    ~Image();
private:
    bool m_isHdr{false};
    int m_width{0};
    int m_height{0};
    int m_numberOfChannels{0};
    float* m_hdrData{nullptr};
    unsigned char* m_data{nullptr};
    texture::TextureFormat m_firstFormat;
    texture::TextureFormat m_secondFormat;
};

ELIX_NAMESPACE_END

#endif //IMAGE_HPP