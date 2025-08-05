#include "VelixGL/GLMesh.hpp"
#include "VelixGL/GLBuffer.hpp"

ELIX_NAMESPACE_BEGIN

GLMesh::GLMesh(const mesh::MeshData& meshData) : m_vertices(meshData.vertices), m_indices(meshData.indices)
{
    bake();
}

void GLMesh::bake()
{
    GLBuffer vbo(GLBuffer::BufferType::Vertex, GLBuffer::BufferUsage::StaticDraw);
    GLBuffer ebo(GLBuffer::BufferType::Index, GLBuffer::BufferUsage::StaticDraw);

    m_vertexArray.create();
    vbo.create();
    ebo.create();

    m_vertexArray.bind();

    vbo.uploadRaw(m_vertices.data(), m_vertices.size() * sizeof(Vertex));

    ebo.uploadRaw(m_indices.data(), m_indices.size() * sizeof(unsigned int));

    m_vertexArray.setAttribute(0, 3, VertexArray::Type::Float, false, sizeof(Vertex),  (void*)offsetof(Vertex, position));
    m_vertexArray.setAttribute(1, 3, VertexArray::Type::Float, false, sizeof(Vertex),  (void*)offsetof(Vertex, normal));
    m_vertexArray.setAttribute(2, 2, VertexArray::Type::Float, false, sizeof(Vertex),  (void*)offsetof(Vertex, textureCoordinates));

    if (hasBones())
    {
        m_vertexArray.setAttribute(3, 3, VertexArray::Type::Float, false, sizeof(Vertex),  (void*)offsetof(Vertex, tangent));
        m_vertexArray.setAttribute(4, 3, VertexArray::Type::Float, false, sizeof(Vertex),  (void*)offsetof(Vertex, bitangent));
        m_vertexArray.setAttribute(5, 4, VertexArray::Type::Int, false, sizeof(Vertex),  (void*)offsetof(Vertex, boneID));
        m_vertexArray.setAttribute(6, 4, VertexArray::Type::Float, false, sizeof(Vertex),  (void*)offsetof(Vertex, weight));
    }

    m_vertexArray.unbind();
    vbo.unbind();
    ebo.unbind();

    m_indicesCount = m_indices.size();
}

[[nodiscard]] bool GLMesh::hasBones() const
{
    for (const auto& vertex : m_vertices)
    if (vertex.boneID[0] != -1)
        return true;

    return false;
}

const VertexArray& GLMesh::getVertexArray() const
{
    return m_vertexArray;
}

const std::vector<Vertex>& GLMesh::getVertices() const
{
    return m_vertices;
}

const std::vector<unsigned int>& GLMesh::getIndices() const
{
    return m_indices;
}


ELIX_NAMESPACE_END