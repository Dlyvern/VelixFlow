#ifndef RENDER_API_HPP
#define RENDER_API_HPP

#include "VelixFlow/DefaultMacros.hpp"

ELIX_NAMESPACE_BEGIN

namespace render
{
    enum class RenderAPI
    {
        OpenGL,
        Vulkan
    };

} //namespace render

ELIX_NAMESPACE_END

#endif //RENDER_API_HPP