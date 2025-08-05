#ifndef IMESH_HPP
#define IMESH_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/Vertex.hpp"

#include <vector>

ELIX_NAMESPACE_BEGIN

namespace mesh
{
    struct MeshData
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };
    
    class IMesh
    {
    public:
        virtual ~IMesh() = default;
    };
}

ELIX_NAMESPACE_END

#endif //IMESH_HPP