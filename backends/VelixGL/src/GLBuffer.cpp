#include "VelixGL/GLBuffer.hpp"
#include <glad/glad.h>

namespace
{
    GLenum toGL(const elix::GLBuffer::BufferType& type)
    {
        switch (type)
        {
            case elix::GLBuffer::BufferType::Vertex: return GL_ARRAY_BUFFER;
            case elix::GLBuffer::BufferType::Index: return GL_ELEMENT_ARRAY_BUFFER;
            case elix::GLBuffer::BufferType::Uniform: return GL_UNIFORM_BUFFER;
        }

        return 0;
    }

    GLenum toGL(const elix::GLBuffer::BufferUsage& type)
    {
        switch (type)
        {
            case elix::GLBuffer::BufferUsage::DynamicDraw: return GL_DYNAMIC_DRAW;
            case elix::GLBuffer::BufferUsage::StaticDraw: return GL_STATIC_DRAW;
            case elix::GLBuffer::BufferUsage::StreamDraw: return GL_STREAM_DRAW;
        }

        return 0;
    }
} //namespace

ELIX_NAMESPACE_BEGIN

GLBuffer::GLBuffer(BufferType type, BufferUsage usage) : m_bufferType(type), m_bufferUsage(usage)
{

}

void GLBuffer::create()
{
    glGenBuffers(1, &m_id);
}

void GLBuffer::bind() const
{
    glBindBuffer(toGL(m_bufferType), m_id);
}

void GLBuffer::unbind()
{
    glBindBuffer(toGL(m_bufferType), 0);
}

void GLBuffer::uploadRaw(const void *data, size_t size)
{
    bind();
    glBufferData(toGL(m_bufferType), size, data, toGL(m_bufferUsage));
    // unbind();
}

GLBuffer::~GLBuffer()
{
    if (m_id)
        glDeleteBuffers(1, &m_id);
}

ELIX_NAMESPACE_END