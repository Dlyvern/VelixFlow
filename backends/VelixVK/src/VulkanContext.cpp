#include "VelixVK/VulkanContext.hpp"
#include "VelixFlow/Logger.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <set>
#include "VelixVK/VKMesh.hpp"
#include "VelixVK/VKTexture.hpp"

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        ELIX_LOG_WARN(std::string(pCallbackData->pMessage));
    else if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
        ELIX_LOG_INFO(std::string(pCallbackData->pMessage));
    else if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
        ELIX_LOG_INFO(std::string(pCallbackData->pMessage));
    else if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        ELIX_LOG_ERROR(std::string(pCallbackData->pMessage));

    return VK_FALSE;
}

ELIX_NAMESPACE_BEGIN

std::shared_ptr<mesh::IMesh> VulkanContext::createMesh(const mesh::MeshData& meshData)
{
    return std::make_shared<mesh::VKMesh>(meshData, this);
}

std::unique_ptr<texture::ITexture> VulkanContext::createTexture(const std::string& path)
{
    return std::make_unique<texture::VKTexture>();
}

void VulkanContext::createDebugger()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo;

    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;

    auto function = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");

    if (function != nullptr)
        if (function(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)
            ELIX_LOG_ERROR("Failed to create debug messenger");       
}

// #define VK_CHECK(result) \
//     if (result != VK_SUCCESS) { \
//         std::cerr << "Vulkan error at " << __FILE__ << ":" << __LINE__ << std::endl; \
//         exit(1); \
//     }

// void testBufferCreation(VkDevice device, VkPhysicalDevice physicalDevice) 
// {
//     VkBufferCreateInfo bufferInfo = {};
//     bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//     bufferInfo.size = 256;
//     bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
//     bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

//     VkBuffer buffer;
//     VkResult result = vkCreateBuffer(device, &bufferInfo, nullptr, &buffer);

//     VK_CHECK(result);

//     std::cout << "vkCreateBuffer succeeded!" << std::endl;

//     VkMemoryRequirements memRequirements;
//     vkGetBufferMemoryRequirements(device, buffer, &memRequirements);
//     std::cout << "Memory requirements:\n"
//               << "  Size: " << memRequirements.size << "\n"
//               << "  Alignment: " << memRequirements.alignment << "\n"
//               << "  MemoryTypeBits: " << memRequirements.memoryTypeBits << std::endl;

//     vkDestroyBuffer(device, buffer, nullptr);
// }

// void test()
// {
//     if (volkInitialize() != VK_SUCCESS) {
//         std::cerr << "Failed to initialize Volk" << std::endl;
//     }

//     VkInstance instance;
//     VkInstanceCreateInfo instanceInfo = {};
//     instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//     instanceInfo.pApplicationInfo = nullptr;
    
//     if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS) {
//         std::cerr << "Failed to create instance" << std::endl;
//     }

//     volkLoadInstance(instance);

//     uint32_t deviceCount = 0;
//     vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
//     std::vector<VkPhysicalDevice> devices(deviceCount);
//     vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    
//     if (devices.empty()) {
//         std::cerr << "No Vulkan devices found" << std::endl;
//         return;
//     }
//     VkPhysicalDevice physicalDevice = devices[0];

//     float queuePriority = 1.0f;
//     VkDeviceQueueCreateInfo queueInfo = {};
//     queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//     queueInfo.queueFamilyIndex = 0;
//     queueInfo.queueCount = 1;
//     queueInfo.pQueuePriorities = &queuePriority;

//     VkDeviceCreateInfo deviceInfo = {};
//     deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//     deviceInfo.queueCreateInfoCount = 1;
//     deviceInfo.pQueueCreateInfos = &queueInfo;

//     VkDevice device;

//     if (vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &device) != VK_SUCCESS) {
//         std::cerr << "Failed to create device" << std::endl;
//         return;
//     }

//     volkLoadDevice(device);

//     std::cout << "vkCreateBuffer pointer: " << (void*)vkCreateBuffer << std::endl;

//     testBufferCreation(device, physicalDevice);

//     vkDestroyDevice(device, nullptr);
//     vkDestroyInstance(instance, nullptr);

//     ELIX_LOG_INFO("Test completed");
// }


void VulkanContext::init(GLFWwindow* window)
{
    if (volkInitialize() != VK_SUCCESS)
        ELIX_LOG_ERROR("Failed to initialize Volk");
    
    m_isValidationLayersEnabled = true;

    createInstance();
    createDebugger();

    createSurface(window);

    pickPhysicalDevice();
    createLogicalDevice();

    createSwapChain(window);
    createImageViews();
}

VulkanContext::QueueFamilyIndices VulkanContext::findQueueFamilies(VkPhysicalDevice device)
{   
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount{0};

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamiles(queueFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamiles.data());

    int i{0};
    VkBool32 presentSupport{false};

    for(const auto& queueFamily : queueFamiles)
    {
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);

        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indices.graphicsFamily = i;

        if(presentSupport)
            indices.presentFamily = i;

        ++i;
    }

    return indices;
}

void VulkanContext::createInstance()
{
    VkApplicationInfo applicationInfo{};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = "VelixFlow";
    applicationInfo.pEngineName = "VelixEngine";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    applicationInfo.apiVersion = VK_API_VERSION_1_3;

    uint32_t glfwExtensionCount{0};

    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    uint32_t extensionCount{0};    

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    //TODO check if all glfw extensions are represented in vulkan extensions
    for(const auto& extension : extensions)
        ELIX_LOG_INFO("Vulkan extension: ", extension.extensionName);    

    for(int index = 0; index < glfwExtensionCount; ++index)
        ELIX_LOG_INFO("GLFW extension: ", glfwExtensions[index]);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &applicationInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    if(m_isValidationLayersEnabled)
    {
        if(!checkValidationLayers())
            ELIX_LOG_ERROR("Validation layers required, but not available");
        else
        {
            createInfo.enabledLayerCount = m_validationLayers.size();
            createInfo.ppEnabledLayerNames = m_validationLayers.data();
        }
    }
    else
        createInfo.enabledLayerCount = 0;

    auto newExtensions = getRequiredExtensions();

    createInfo.enabledExtensionCount = newExtensions.size();
    createInfo.ppEnabledExtensionNames = newExtensions.data();

    if(vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
        throw std::runtime_error("Failed to create Vulkan instance");

    volkLoadInstance(m_instance);
}

std::vector<const char*> VulkanContext::getRequiredExtensions()
{
    uint32_t glfwExtensionCount{0};

    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if(m_isValidationLayersEnabled)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return extensions;
}

bool VulkanContext::checkValidationLayers()
{
    uint32_t layerCount;

    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);

    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    bool layerFound{false};

    for(const auto& layerName : m_validationLayers)
    {
        for(const auto& availableLayer : availableLayers)
        {
            if(strcmp(layerName, availableLayer.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }
    }

    return layerFound;
}

bool VulkanContext::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount{0};

    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);

    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(m_deviceExtensions.begin(), m_deviceExtensions.end());

    for(const auto& extension : availableExtensions)
        requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
}

bool VulkanContext::isDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = findQueueFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate{false};

    if(extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);

        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;

    // VkPhysicalDeviceProperties deviceProperties;

    // vkGetPhysicalDeviceProperties(device, &deviceProperties);

    // VkPhysicalDeviceFeatures deviceFeatures;

    // vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    // return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
}

void VulkanContext::pickPhysicalDevice()
{
    uint32_t deviceCount{0};

    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

    if(deviceCount == 0)
        throw std::runtime_error("Failed to find GPUs with Vulkan support");

    std::vector<VkPhysicalDevice> devices(deviceCount);

    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

    for(const auto& device : devices)
    {
        if(isDeviceSuitable(device))
        {
            m_physicalDevice = device;
            break;
        }
    }

    if(!m_physicalDevice)
        throw std::runtime_error("Failed to find a suitable GPU");
}


void VulkanContext::createLogicalDevice()
{
    QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority{1.0f};

    for(uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    deviceFeatures.samplerAnisotropy = VK_TRUE;
    deviceFeatures.fillModeNonSolid = VK_TRUE;

    VkDeviceCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = queueCreateInfos.size();
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = m_deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = m_deviceExtensions.data();
    createInfo.enabledLayerCount = 0;

    if(m_isValidationLayersEnabled)
    {
        createInfo.enabledLayerCount = m_validationLayers.size();
        createInfo.ppEnabledLayerNames = m_validationLayers.data();
    }

    if(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
        throw std::runtime_error("Failed to create logical device");

    vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
    
    volkLoadDevice(m_device);
}

void VulkanContext::createSurface(GLFWwindow* window)
{
    if(glfwCreateWindowSurface(m_instance, window, nullptr, &m_surface) != VK_SUCCESS)
        throw std::runtime_error("Failed to create window surface");
}


VkExtent2D VulkanContext::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window)
{
    if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        return capabilities.currentExtent;

    int width, height;

    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actualExtent{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
}

VkSurfaceFormatKHR VulkanContext::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for(const auto& availableFormat : availableFormats)
        if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return availableFormat;

    return availableFormats[0];
}

VkPresentModeKHR VulkanContext::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for(const auto& availablePresentMode : availablePresentModes)
        if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            return availablePresentMode;

    return VK_PRESENT_MODE_FIFO_KHR;
}


void VulkanContext::createSwapChain(GLFWwindow* window)
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    
    if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        imageCount = swapChainSupport.capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);

    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if(indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if(vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
        throw std::runtime_error("Failed to create swap chain");

    vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, nullptr);
    m_swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, m_swapChainImages.data());

    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent = extent;
}

void VulkanContext::createImageViews()
{
    m_swapChainImageViews.resize(m_swapChainImages.size());

    for(size_t index = 0; index < m_swapChainImages.size(); ++index)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_swapChainImages[index];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = m_swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if(vkCreateImageView(m_device, &createInfo, nullptr, &m_swapChainImageViews[index]) != VK_SUCCESS)
            throw std::runtime_error("Failed to create image views");
    }
}

VulkanContext::SwapChainSupportDetails VulkanContext::querySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);

    uint32_t formatCount{0};

    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);

    if(formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount{0};

    vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);

    if(presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

void VulkanContext::createRenderPass()
{
    VkRenderPassCreateInfo createInfo;

    vkCreateRenderPass(m_device, &createInfo, nullptr, &m_renderPass);
}

void VulkanContext::cleanup()
{
    if(m_surface)
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    if(m_instance)
        vkDestroyInstance(m_instance, nullptr);
    if(m_swapChain)
        vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);

    for(auto imageView : m_swapChainImageViews)
        vkDestroyImageView(m_device, imageView, nullptr);

    if(m_device)
        vkDestroyDevice(m_device, nullptr);
}

VkInstance VulkanContext::getInstance() const
{
    return m_instance;
}

VkSurfaceKHR VulkanContext::getSurface() const
{
    return m_surface;
}

VkPhysicalDevice VulkanContext::getPhysicalDevice() const
{
    return m_physicalDevice;
}

VkDevice VulkanContext::getDevice() const
{
    return m_device;
}

VkSwapchainKHR VulkanContext::getSwapchain() const
{
    return m_swapChain;
}

VkQueue VulkanContext::getGraphicsQueue() const
{
    return m_graphicsQueue;
}

VkRenderPass VulkanContext::getRenderPass() const
{
    return m_renderPass;
}

ELIX_NAMESPACE_END