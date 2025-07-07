#include "Mesh.hpp"
#include "Buffer.hpp"
#include "DrawCall.hpp"

ELIX_NAMESPACE_BEGIN

Mesh::Mesh(const std::vector<common::Vertex> &vertices, const std::vector<unsigned int> &indices) : m_vertices(vertices), m_indices(indices)
{
    Buffer vbo(Buffer::BufferType::Vertex, Buffer::BufferUsage::StaticDraw);
    Buffer ebo(Buffer::BufferType::Index, Buffer::BufferUsage::StaticDraw);

    m_vertexArray.create();
    vbo.create();
    ebo.create();

    m_vertexArray.bind();

    vbo.uploadRaw(m_vertices.data(), m_vertices.size() * sizeof(common::Vertex));

    ebo.uploadRaw(m_indices.data(), m_indices.size() * sizeof(unsigned int));

    m_vertexArray.setAttribute(0, 3, VertexArray::Type::Float, false, sizeof(common::Vertex),  (void*)offsetof(common::Vertex, position));
    m_vertexArray.setAttribute(1, 3, VertexArray::Type::Float, false, sizeof(common::Vertex),  (void*)offsetof(common::Vertex, normal));
    m_vertexArray.setAttribute(2, 2, VertexArray::Type::Float, false, sizeof(common::Vertex),  (void*)offsetof(common::Vertex, textureCoordinates));

    if (hasBones())
    {
        m_vertexArray.setAttribute(3, 3, VertexArray::Type::Float, false, sizeof(common::Vertex),  (void*)offsetof(common::Vertex, tangent));
        m_vertexArray.setAttribute(4, 3, VertexArray::Type::Float, false, sizeof(common::Vertex),  (void*)offsetof(common::Vertex, bitangent));
        m_vertexArray.setAttribute(5, 4, VertexArray::Type::Int, false, sizeof(common::Vertex),  (void*)offsetof(common::Vertex, boneID));
        m_vertexArray.setAttribute(6, 4, VertexArray::Type::Float, false, sizeof(common::Vertex),  (void*)offsetof(common::Vertex, weight));
    }

    m_vertexArray.unbind();
    vbo.unbind();
    ebo.unbind();

    m_indicesCount = m_indices.size();
}

const std::vector<common::Vertex>& Mesh::getVertices() const
{
    return m_vertices;
}

const std::vector<unsigned int>& Mesh::getIndices() const
{
    return m_indices;
}

bool Mesh::hasBones() const
{
    for (const auto& vertex : m_vertices)
        if (vertex.boneID[0] != -1)
            return true;

    return false;
}

void Mesh::draw() const
{
    m_vertexArray.bind();
    DrawCall::draw(DrawCall::DrawMode::TRIANGLES, m_indicesCount, DrawCall::DrawType::UNSIGNED_INT, nullptr);
    m_vertexArray.unbind();
}

ELIX_NAMESPACE_END
