#include "VelixGL/GLTexture.hpp"
#include "VelixGL/GLConverter.hpp"
#include "VelixFlow/Logger.hpp"

#include <filesystem>

ELIX_NAMESPACE_BEGIN

namespace texture
{
    GLTexture::GLTexture(const std::string& name) : m_name(name) {}

    void GLTexture::bind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(gfx::OpenGL::toGL(textureUsage_), m_id);
    }

    unsigned int GLTexture::getId() const
    {
        return m_id;
    }

    const std::string& GLTexture::getName() const
    {
        return m_name;
    }

    void GLTexture::unbind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(gfx::OpenGL::toGL(textureUsage_), 0);
    }

    bool GLTexture::load(const std::string& path)
    {
        if(!m_image.load(path, false))
            return false;

        float numberOfChannels = m_image.getNumberOfChannels();
        TextureFormat format;

        if (numberOfChannels == 4)
            format = TextureFormat::RGBA;
        else if (numberOfChannels == 3)
            format = TextureFormat::RGB;
        else if (numberOfChannels == 1)
            format = TextureFormat::RED;

        m_image.setFirstFormat(format);
        m_image.setSecondFormat(format);

        const std::filesystem::path file(path);

        m_name = file.filename().string();

        return true;
    }

    void GLTexture::setImage(const Image& image)
    {
        m_image = image;
    }

    unsigned int& GLTexture::getId()
    {
        return m_id;
    }

    void GLTexture::upload()
    {
        glGenTextures(1, &m_id);

        glBindTexture(gfx::OpenGL::toGL(textureUsage_), m_id);

        for(const auto& param : m_textureParameters)
            glTexParameteri(gfx::OpenGL::toGL(textureUsage_), gfx::OpenGL::toGL(param.type), gfx::OpenGL::toGL(param.value));

        if(m_image.isHdr())
        {
            glTexImage2D(GL_TEXTURE_2D, 0, gfx::OpenGL::toGL(m_image.getFirstFormat()), m_image.getWidth(), m_image.getHeight(), 0,
            gfx::OpenGL::toGL(m_image.getSecondFormat()), GL_FLOAT, m_image.getHdrData());
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, gfx::OpenGL::toGL(m_image.getFirstFormat()), m_image.getWidth(), m_image.getHeight(), 0,
            gfx::OpenGL::toGL(m_image.getSecondFormat()), GL_UNSIGNED_BYTE, m_image.getData());
        }

        glGenerateMipmap(gfx::OpenGL::toGL(textureUsage_));

        glBindTexture(gfx::OpenGL::toGL(textureUsage_), 0);

        m_isBaked = true;
    }

    bool GLTexture::isUploaded() const
    {
        return m_isBaked;
    }

    void GLTexture::addDefaultParameters()
    {
        addParameter(ParameterType::MIN_FILTER, ParameterValue::LINEAR);

        addParameter(ParameterType::MAG_FILTER, ParameterValue::LINEAR);

        addParameter(ParameterType::WRAP_S, ParameterValue::REPEAT);

        addParameter(ParameterType::WRAP_T, ParameterValue::REPEAT);
    }

    void GLTexture::addParameter(ParameterType type, ParameterValue value)
    {
        Parameter parameter;
        parameter.type = type;
        parameter.value = value;

        m_textureParameters.push_back(parameter);
    }

    GLTexture::~GLTexture()
    {
        if(m_id)
            glDeleteTextures(1, &m_id);
    }

} //namespace texture

ELIX_NAMESPACE_END