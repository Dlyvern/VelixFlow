#ifndef GL_MESH_HPP
#define GL_MESH_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/Vertex.hpp"
#include "VelixGL/VertexArray.hpp"
#include "VelixFlow/RenderAPI/Interface/IMesh.hpp"

#include <vector>

ELIX_NAMESPACE_BEGIN

class GLMesh : public mesh::IMesh
{
public:
    GLMesh(const mesh::MeshData& meshData);

    void bake();

    [[nodiscard]] bool hasBones() const;

    const VertexArray& getVertexArray() const;

    const std::vector<Vertex>& getVertices() const;
    const std::vector<unsigned int>& getIndices() const;
private:
    uint32_t m_indicesCount{0};
    VertexArray m_vertexArray;

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
};

ELIX_NAMESPACE_END

#endif //GL_MESH_HPP