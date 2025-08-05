#ifndef GL_CONVERTER_HPP
#define GL_CONVERTER_HPP

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/TextureParameters.hpp"
#include <glad/glad.h>


ELIX_NAMESPACE_BEGIN

namespace gfx::OpenGL
{
    inline GLenum toGL(texture::ParameterType type)
    {
        switch(type)
        {
            case texture::ParameterType::MAG_FILTER: return GL_TEXTURE_MAG_FILTER;

            case texture::ParameterType::MIN_FILTER: return GL_TEXTURE_MIN_FILTER;

            case texture::ParameterType::WRAP_S: return GL_TEXTURE_WRAP_S;

            case texture::ParameterType::WRAP_T: return GL_TEXTURE_WRAP_T;
        }

        return GL_NONE;
    }

    inline GLenum toGL(texture::ParameterValue value)
    {
        switch(value)
        {
            case texture::ParameterValue::LINEAR: return GL_LINEAR;
            case texture::ParameterValue::REPEAT: return GL_REPEAT;
        }

        return GL_NONE;
    }

    inline GLenum toGL(texture::TextureFormat format)
    {
        switch (format) {
            case texture::TextureFormat::RGB: return GL_RGB;
            case texture::TextureFormat::RGBA: return GL_RGBA;
            case texture::TextureFormat::RED: return GL_RED;
            case texture::TextureFormat::RGB16F: return GL_RGB16F;
            case texture::TextureFormat::DepthComponent: return GL_DEPTH_COMPONENT;
        }
        return GL_RGB;
    }


    inline GLenum toGL(texture::TextureUsage usage)
    {
        switch(usage)
        {
            case texture::TextureUsage::Standard2D: return GL_TEXTURE_2D;
            case texture::TextureUsage::RenderTarget: return GL_NONE;
            case texture::TextureUsage::CUBEMAP: return GL_TEXTURE_CUBE_MAP;
         }

        return GL_NONE;
    }
} //gfx::OpenGL

ELIX_NAMESPACE_END

#endif //GL_CONVERTER_HPP