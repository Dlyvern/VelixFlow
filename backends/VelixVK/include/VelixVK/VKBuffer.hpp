#ifndef VK_BUFFER_HPP
#define VK_BUFFER_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "vulkan/vulkan.h"

ELIX_NAMESPACE_BEGIN

class VKBuffer
{
public:
    VKBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags flags, VkMemoryPropertyFlags properties);

    ~VKBuffer();

    void upload(const void* data, VkDeviceSize size);

    VkBuffer getBuffer() const;

private:
    VkDevice m_device = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkBuffer m_buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_bufferMemory = VK_NULL_HANDLE;

    bool createBuffer(VkDeviceSize size, VkBufferUsageFlags flags, VkMemoryPropertyFlags properties);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags flags);
};

ELIX_NAMESPACE_END

#endif //VK_BUFFER_HPP