#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "VelixFlow/Vertex.hpp"
#include <vector>

namespace primitive
{
    std::vector<elix::Vertex> cubeVertices =
    {
        {{-0.5f, -0.5f,  0.5f}, {0, 0, 1}},
        {{ 0.5f, -0.5f,  0.5f}, {0, 0, 1}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 0, 1}},
        {{-0.5f,  0.5f,  0.5f}, {0, 0, 1}},
        {{-0.5f, -0.5f, -0.5f}, {0, 0, -1}},
        {{ 0.5f, -0.5f, -0.5f}, {0, 0, -1}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 0, -1}},
        {{-0.5f,  0.5f, -0.5f}, {0, 0, -1}},
    };

    std::vector<uint32_t> cubeIndices = 
    {
        0, 1, 2, 2, 3, 0,
        1, 5, 6, 6, 2, 1,
        5, 4, 7, 7, 6, 5,
        4, 0, 3, 3, 7, 4,
        3, 2, 6, 6, 7, 3,
        4, 5, 1, 1, 0, 4
    };

} //namespace primitive

#endif //PRIMITIVE_HPP