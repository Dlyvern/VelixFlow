#include "VelixVK/VulkanContext.hpp"

#include "VelixVK/VKMesh.hpp"
#include "VelixFlow/MeshFactory.hpp"

#include "VelixFlow/Logger.hpp"

ELIX_NAMESPACE_BEGIN

namespace mesh
{
    VKMesh::VKMesh(const MeshData& meshData, IRenderContext* context)
    {
        if(auto vulkanContext = dynamic_cast<VulkanContext*>(context))
        {
            m_indicesCount = meshData.indices.size();

            VkDeviceSize vertexSize = sizeof(Vertex) * meshData.vertices.size();
            VkDeviceSize indicesSize = sizeof(uint32_t) * m_indicesCount;

            VkDevice device = vulkanContext->getDevice();
            VkPhysicalDevice physicalDevice = vulkanContext->getPhysicalDevice();

            m_vertexBuffer = std::make_shared<VKBuffer>(device, physicalDevice, vertexSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | 
            VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            m_vertexBuffer->upload(meshData.vertices.data(), vertexSize);

            m_indicesBuffer = std::make_shared<VKBuffer>(device, physicalDevice, indicesSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | 
            VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            m_indicesBuffer->upload(meshData.indices.data(), indicesSize);
        }
        else
            ELIX_LOG_ERROR("Failed to get vulkan context");
    }

    void VKMesh::bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer vertexBuffers[] = {m_vertexBuffer->getBuffer()};
        VkDeviceSize offsets[] = {0};

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, m_indicesBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }

    void VKMesh::draw(VkCommandBuffer commandBuffer)
    {
        vkCmdDrawIndexed(commandBuffer, m_indicesCount, 1, 0, 0, 0);
    }
}

ELIX_NAMESPACE_END