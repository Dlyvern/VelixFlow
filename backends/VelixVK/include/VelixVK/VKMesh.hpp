#ifndef VK_MESH_HPP
#define VK_MESH_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixVK/VKBuffer.hpp"
#include "VelixFlow/Vertex.hpp"
#include "VelixFlow/RenderAPI/Interface/IMesh.hpp"

#include "VelixFlow/RenderAPI/Interface/IRenderContext.hpp"


#include <vector>
#include <memory>

ELIX_NAMESPACE_BEGIN

namespace mesh
{
    class VKMesh : public mesh::IMesh
    {
    public:
        VKMesh(const MeshData& meshData, IRenderContext* context);

        void bind(VkCommandBuffer commandBuffer);

        void draw(VkCommandBuffer commandBuffer);

    private:
        std::shared_ptr<VKBuffer> m_vertexBuffer;
        std::shared_ptr<VKBuffer> m_indicesBuffer;

        uint32_t m_indicesCount{0};
    };
}

ELIX_NAMESPACE_END

#endif //VK_MESH_HPP