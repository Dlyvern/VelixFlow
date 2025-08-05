#ifndef TEXTURE_PARAMETERS_HPP
#define TEXTURE_PARAMETERS_HPP

#include "VelixFlow/DefaultMacros.hpp"

ELIX_NAMESPACE_BEGIN

namespace texture
{
    enum class ParameterType
    {
        MIN_FILTER,
        MAG_FILTER,
        WRAP_S,
        WRAP_T 
    };

    enum class TextureFormat
    {
        RGB,
        RGB16F,
        RGBA,
        RED,
        SRGB,
        SRGBA,
        DepthComponent
    };

    enum class TextureType
    {
        Diffuse,
        Specular,
        Normal,
        Metallic,
        Roughness,
        AO,
        Emissive,
        Height,
        Glossiness,
        Opacity,
        Depth2D,
        Undefined
    };

    enum class TextureUsage
    {
        Standard2D,
        RenderTarget,
        CUBEMAP
    };


    enum class ParameterValue
    {
        LINEAR,
        REPEAT,
        CLAMP_TO_EDGE
    };

    struct Parameter
    {
        ParameterType type;
        ParameterValue value;
    };

} //namespace texture

ELIX_NAMESPACE_END


#endif //TEXTURE_PARAMETERS_HPP