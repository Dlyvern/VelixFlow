#include "VelixVK/VKBuffer.hpp"
#include "VelixFlow/Logger.hpp"
#include <cstring>

ELIX_NAMESPACE_BEGIN

VKBuffer::VKBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags flags,
VkMemoryPropertyFlags properties) :
m_device(device),
m_physicalDevice(physicalDevice)
{
    createBuffer(size, flags, properties);
}

VKBuffer::~VKBuffer()
{
    if(m_buffer != VK_NULL_HANDLE)
        vkDestroyBuffer(m_device, m_buffer, nullptr);

    if(m_bufferMemory != VK_NULL_HANDLE)
        vkFreeMemory(m_device, m_bufferMemory, nullptr);
}

bool VKBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags flags, VkMemoryPropertyFlags properties)
{
    //TODO 
    return true;
    //TODO
    if (size == 0)
    {
        ELIX_LOG_ERROR("Attempted to create buffer with zero size");
        return false;
    }

    if (m_device == VK_NULL_HANDLE) 
    {
        ELIX_LOG_ERROR("Vulkan device handle is null");
        return false;
    }

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = flags;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;



    {
        ELIX_LOG_INFO("Executing test");
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = 256;
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VkBuffer buffer;
        VkResult result = vkCreateBuffer(m_device, &bufferInfo, nullptr, &buffer);

        if(result != VK_SUCCESS)
            ELIX_LOG_ERROR("Failed test");

        std::cout << "vkCreateBuffer succeeded!" << std::endl;

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(m_device, buffer, &memRequirements);

        std::cout << "Memory requirements:\n"
                << "  Size: " << memRequirements.size << "\n"
                << "  Alignment: " << memRequirements.alignment << "\n"
                << "  MemoryTypeBits: " << memRequirements.memoryTypeBits << std::endl;

        vkDestroyBuffer(m_device, buffer, nullptr);
    }



    if(vkCreateBuffer(m_device, &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS)
    {
        ELIX_LOG_ERROR("Failed to create vulkan buffer");
        return false;
    }
    else
    {
        ELIX_LOG_INFO("Created buffer");
    }

    VkMemoryRequirements memoryRequirements{};
    vkGetBufferMemoryRequirements(m_device, m_buffer, &memoryRequirements);

    VkMemoryAllocateInfo allocateInfo{};
    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize = memoryRequirements.size;
    allocateInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, properties);

    if(vkAllocateMemory(m_device, &allocateInfo, nullptr, &m_bufferMemory) != VK_SUCCESS)
    {
        ELIX_LOG_ERROR("Failed to allocate Vulkan buffer memory");
        return false;
    }
    else
    {
        ELIX_LOG_INFO("Allocated buffer memory");
    }

    vkBindBufferMemory(m_device, m_buffer, m_bufferMemory, 0);

    return true;
}

void VKBuffer::upload(const void* data, VkDeviceSize size)
{
    //TODO 
    return;
    //TODO

    void* dst;

    vkMapMemory(m_device, m_bufferMemory, 0, size, 0, &dst);
    std::memcpy(dst, data, static_cast<size_t>(size));
    vkUnmapMemory(m_device, m_bufferMemory);
}

uint32_t VKBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags flags)
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memoryProperties);

    for(uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
        if(typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & flags) == flags)
            return i;

    ELIX_LOG_ERROR("Failed to find suitable memory type for buffer");

    return 0;
}

VkBuffer VKBuffer::getBuffer() const
{
    return m_buffer;
}

ELIX_NAMESPACE_END