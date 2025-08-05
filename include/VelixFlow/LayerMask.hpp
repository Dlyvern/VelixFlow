#ifndef LAYER_MASK_HPP
#define LAYER_MASK_HPP

#include "VelixFlow/DefaultMacros.hpp"

ELIX_NAMESPACE_BEGIN

//Maybe 32 is way too big
enum LayerMask : uint16_t
{
    DEFAULT = 1 << 0,
    GROUND = 1 << 1,
    PLAYER = 1 << 2,
    INTERACTABLE = 1 << 3,
};



ELIX_NAMESPACE_END

#endif //LAYER_MASK_HPP