#ifndef GL_BUFFER_HPP
#define GL_BUFFER_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/Interface/IBuffer.hpp"

#include <cstddef>

ELIX_NAMESPACE_BEGIN

class GLBuffer : public IBuffer
{
public:
    enum class BufferType
    {
        Vertex,
        Index,
        Uniform
    };

    enum class BufferUsage
    {
        StaticDraw,
        DynamicDraw,
        StreamDraw,
    };

    GLBuffer(BufferType type, BufferUsage usage);

    void create();

    void bind() const;

    void unbind();

    void uploadRaw(const void* data, size_t size);

    ~GLBuffer();
private:
    unsigned int m_id{0};
    BufferType m_bufferType;
    BufferUsage m_bufferUsage;
};

ELIX_NAMESPACE_END

#endif //GL_BUFFER_HPP