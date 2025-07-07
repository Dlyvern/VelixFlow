#include "Texture.hpp"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <filesystem>
#include <iostream>

#include "Logger.hpp"

namespace
{
    GLenum toGL(elix::Texture::ParameterType type)
    {
        switch(type)
        {
            case elix::Texture::ParameterType::MAG_FILTER: return GL_TEXTURE_MAG_FILTER;

            case elix::Texture::ParameterType::MIN_FILTER: return GL_TEXTURE_MIN_FILTER;

            case elix::Texture::ParameterType::WRAP_S: return GL_TEXTURE_WRAP_S;

            case elix::Texture::ParameterType::WRAP_T: return GL_TEXTURE_WRAP_T;
        }

        return GL_NONE;
    }


    GLenum toGL(elix::Texture::ParameterValue value)
    {
        switch(value)
        {
            case elix::Texture::ParameterValue::LINEAR: return GL_LINEAR;
            case elix::Texture::ParameterValue::REPEAT: return GL_REPEAT;
        }

        return GL_NONE;
    }

    GLenum toGL(elix::Texture::TextureFormat format)
    {
        switch (format) {
            case elix::Texture::TextureFormat::RGB: return GL_RGB;
            case elix::Texture::TextureFormat::RGBA: return GL_RGBA;
            case elix::Texture::TextureFormat::RED: return GL_RED;
            case elix::Texture::TextureFormat::DepthComponent: return GL_DEPTH_COMPONENT;
        }
        return GL_RGB;
    }


    GLenum toGL(elix::Texture::TextureUsage usage)
    {
        switch(usage)
        {
            case elix::Texture::TextureUsage::Standard2D: return GL_TEXTURE_2D;
            case elix::Texture::Texture::TextureUsage::RenderTarget: return GL_NONE;
         }

        return GL_NONE;
    }
}

elix::Texture::Texture() = default;

elix::Texture::Texture(const std::string &filePath)
{
    load(filePath);
}

elix::Texture::TextureData elix::Texture::loadImage(const std::string& filePath, bool flipVertically)
{
    stbi_set_flip_vertically_on_load(flipVertically);

    elix::Texture::TextureData textureData{};

    textureData.data = stbi_load(filePath.data(), &textureData.width, &textureData.height, &textureData.numberOfChannels, 0);

    if(!textureData.data)
        ELIX_LOG_ERROR("Failed to load a texture: ", filePath);

    return textureData;
}

void elix::Texture::load(const std::string &filePath)
{
    m_textureData = loadImage(filePath, false);

    if (m_textureData.numberOfChannels == 4)
        m_textureData.format = elix::Texture::TextureFormat::RGBA;
    else if (m_textureData.numberOfChannels == 3)
        m_textureData.format = elix::Texture::TextureFormat::RGB;
    else if (m_textureData.numberOfChannels == 1)
        m_textureData.format = elix::Texture::TextureFormat::RED;

    const std::filesystem::path file(filePath);
    m_name = file.filename().string();
}

void elix::Texture::addParameter(elix::Texture::TextureUsage usage, elix::Texture::ParameterType type, elix::Texture::ParameterValue value)
{
    elix::Texture::Parameter parameter;
    parameter.type = type;
    parameter.usage = usage;
    parameter.value = value;

    m_textureParameters.push_back(parameter);
}

const std::string& elix::Texture::getName() const
{
    return m_name;
}

unsigned int elix::Texture::getId() const
{
    return m_id;
}

void elix::Texture::addDefaultParameters()
{
    addParameter(TextureUsage::Standard2D, ParameterType::MIN_FILTER, ParameterValue::LINEAR);

    addParameter(TextureUsage::Standard2D, ParameterType::MAG_FILTER, ParameterValue::LINEAR);

    addParameter(TextureUsage::Standard2D, ParameterType::WRAP_S, ParameterValue::REPEAT);

    addParameter(TextureUsage::Standard2D, ParameterType::WRAP_T, ParameterValue::REPEAT);
}

void elix::Texture::create()
{
    glGenTextures(1, &m_id);
}

void elix::Texture::bake()
{
    glBindTexture(GL_TEXTURE_2D, m_id);

    for(const auto& param : m_textureParameters)
        glTexParameteri(toGL(param.usage), toGL(param.type), toGL(param.value));

    glTexImage2D(GL_TEXTURE_2D, 0, toGL(m_textureData.format), m_textureData.width, m_textureData.height, 0, toGL(m_textureData.format), GL_UNSIGNED_BYTE, m_textureData.data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
   
    if(m_textureData.data)
        stbi_image_free(m_textureData.data);

    m_textureData.data = nullptr;

    m_isBaked = true;
}

bool elix::Texture::isBaked() const
{
    return m_isBaked;
}

void elix::Texture::unbind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void elix::Texture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

elix::Texture::~Texture()
{
    if (m_id != 0)
        glDeleteTextures(1, &m_id);
}
