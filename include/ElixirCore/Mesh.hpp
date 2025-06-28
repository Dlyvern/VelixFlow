#ifndef MESH_HPP
#define MESH_HPP

#include "Common.hpp"
#include "VertexArray.hpp"

namespace elix
{
    class Mesh
    {
    public:
        Mesh(const std::vector<common::Vertex>& vertices, const std::vector<unsigned int>& indices);

        void draw() const;

        [[nodiscard]] bool hasBones() const;
    private:
        uint32_t m_indicesCount{0};

        elix::VertexArray m_vertexArray;

        std::vector<common::Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
    };
}

#endif //MESH_HPP
