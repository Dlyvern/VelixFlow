#include "FrameBuffer.hpp"
#include <glad/glad.h>
#include "Logger.hpp"

namespace
{
    GLenum toGL(const elix::FrameBuffer::InternalFormat& internalFormat)
    {
        switch (internalFormat)
        {
            case  elix::FrameBuffer::InternalFormat::DEPTH24: return GL_DEPTH_COMPONENT24;
            case elix::FrameBuffer::InternalFormat::DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
        }

        return GL_NONE;
    }

    GLenum toGL(const elix::FrameBuffer::Attachment& attachment)
    {
        switch (attachment)
        {
            case elix::FrameBuffer::Attachment::DEPTH: return GL_DEPTH_ATTACHMENT;
            case elix::FrameBuffer::Attachment::DEPTH_STENCIL: return GL_DEPTH_STENCIL_ATTACHMENT;
        }

        return GL_NONE;
    }
}

int elix::FrameBuffer::getWidth() const {return m_width;}

int elix::FrameBuffer::getHeight() const {return m_height;}

void elix::FrameBuffer::resize(int newWidth, int newHeight)
{
    if (newWidth == m_width && newHeight == m_height)
        return;

    m_width = newWidth;
    m_height = newHeight;

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    for (size_t i = 0; i < m_textureIds.size(); ++i)
    {
        glBindTexture(GL_TEXTURE_2D, m_textureIds[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newWidth, newHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(i), GL_TEXTURE_2D, m_textureIds[i], 0);
    }

    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, toGL(m_internalFormat), newWidth, newHeight);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void elix::FrameBuffer::create(int width, int height, InternalFormat format, bool texture)
{
    m_width = width;
    m_height = height;
    m_internalFormat = format;

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    if(texture)
    {
        GLuint colorTex;
        glGenTextures(1, &colorTex);
        glBindTexture(GL_TEXTURE_2D, colorTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);
        m_textureIds.push_back(colorTex);
    }

    glGenRenderbuffers(1, &m_rbo);

    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, toGL(m_internalFormat), m_width, m_height);


    for (const auto& attach : m_attachments)
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, toGL(attach), GL_RENDERBUFFER, m_rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        ELIX_LOG_ERROR("Buffer is not completed");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int elix::FrameBuffer::getTexture(int index) const
{
    return m_textureIds.at(index);
}

void elix::FrameBuffer::addTextureId(unsigned int id)
{
    m_textureIds.push_back(id);
}

void elix::FrameBuffer::addAttachment(Attachment attachment)
{
    if (m_attachments.find(attachment) == m_attachments.end())
        m_attachments.insert(attachment);
}

void elix::FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void elix::FrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

elix::FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteRenderbuffers(1, &m_rbo);


    for (GLuint tex : m_textureIds)
        glDeleteTextures(1, &tex);
}
