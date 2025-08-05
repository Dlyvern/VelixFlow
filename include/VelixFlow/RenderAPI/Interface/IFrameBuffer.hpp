#ifndef IFRAMEBUFFER_HPP
#define IFRAMEBUFFER_HPP

#include "VelixFlow/DefaultMacros.hpp"

ELIX_NAMESPACE_BEGIN


class IFrameBuffer
{
public:
    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual ~IFrameBuffer() = default;
};

ELIX_NAMESPACE_END


#endif //IFRAMEBUFFER_HPP