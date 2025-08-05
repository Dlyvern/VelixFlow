#include "VelixFlow/Image.hpp"
#include "VelixFlow/Logger.hpp"

#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

ELIX_NAMESPACE_BEGIN

bool Image::isHdr() const
{
    return m_isHdr;
}

bool Image::load(const std::string& pathToImage, bool flip)
{
    stbi_set_flip_vertically_on_load(flip);

    const std::string ext = std::filesystem::path(pathToImage).extension().string();

    if(ext == ".hdr")
    {   
        m_isHdr = true;

        m_hdrData = stbi_loadf(pathToImage.c_str(), &m_width, &m_height, &m_numberOfChannels, 0);

        if (!m_hdrData)
        {
            ELIX_LOG_ERROR("Failed to load HDR image: ", pathToImage);
            return false;
        }
    }
    else
    {
        m_data = stbi_load(pathToImage.data(), &m_width, &m_height, &m_numberOfChannels, 0);

        if(!m_data)
        {
            ELIX_LOG_ERROR("Failed to load image: ", pathToImage);
            return false;
        }
    }

    return true;
}

float* Image::getHdrData()
{
    return m_hdrData;
}

void Image::free()
{
    if(m_data)
        stbi_image_free(m_data);
    if(m_hdrData)
        stbi_image_free(m_hdrData);

    m_hdrData = nullptr;
    m_data = nullptr;
}

int Image::getWidth() const
{
    return m_width;
}

int Image::getHeight() const
{
    return m_height;
}

int Image::getNumberOfChannels() const
{
    return m_numberOfChannels;
}

unsigned char* Image::getData()
{
    return m_data;
}

texture::TextureFormat Image::getFirstFormat() const
{
    return m_firstFormat;
}

texture::TextureFormat Image::getSecondFormat() const
{
    return m_secondFormat;
}

void Image::setWidth(int width)
{
    m_width = width;
}

void Image::setHeight(int height)
{
    m_height = height;
}

void Image::setNumberOfChannels(int numberOfChannels)
{
    m_numberOfChannels = numberOfChannels;
}

void Image::setData(unsigned char* data)
{
    m_data = data;
}

void Image::setFirstFormat(texture::TextureFormat format)
{
    m_firstFormat = format;
}

void Image::setSecondFormat(texture::TextureFormat format)
{
    m_secondFormat = format;
}

Image::~Image()
{
    free();
}

ELIX_NAMESPACE_END