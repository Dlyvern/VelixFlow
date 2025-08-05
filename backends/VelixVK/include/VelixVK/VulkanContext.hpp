#ifndef VULKAN_CONTEXT_HPP
#define VULKAN_CONTEXT_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include <volk.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#if defined(_WIN32)
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <GLFW/glfw3native.h>
#elif defined(__linux__)
    #define GLFW_EXPOSE_NATIVE_X11
    #include <GLFW/glfw3native.h>
#endif

#include <vector>

#include "VelixFlow/RenderAPI/Interface/IRenderContext.hpp"

#include <optional>

ELIX_NAMESPACE_BEGIN

class VulkanContext : public IRenderContext
{
public:
    void init(GLFWwindow* window);

    void cleanup();

    VkInstance getInstance() const;
    VkSurfaceKHR getSurface() const;
    VkPhysicalDevice getPhysicalDevice() const;
    VkDevice getDevice() const;
    VkSwapchainKHR getSwapchain() const;
    VkQueue getGraphicsQueue() const;
    VkRenderPass getRenderPass() const;

    std::shared_ptr<mesh::IMesh> createMesh(const mesh::MeshData& meshData) override;
    std::unique_ptr<texture::ITexture> createTexture(const std::string& path) override;

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() const
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

private:
    void createInstance();
    void createSurface(GLFWwindow* window);
    void createLogicalDevice();
    void createSwapChain(GLFWwindow* window);
    void createDebugger();
    void createRenderPass();
    void createImageViews();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    void pickPhysicalDevice();

    bool isDeviceSuitable(VkPhysicalDevice device);

    bool checkValidationLayers();

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    std::vector<const char*> getRequiredExtensions();

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    bool m_isValidationLayersEnabled{false};

    const std::vector<const char*> m_validationLayers{"VK_LAYER_KHRONOS_validation"};

    VkDebugUtilsMessengerEXT m_debugMessenger;

    const std::vector<const char*> m_deviceExtensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkSwapchainKHR m_swapChain{};
    std::vector<VkImage> m_swapChainImages;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
    std::vector<VkImageView> m_swapChainImageViews;

    VkRenderPass m_renderPass = VK_NULL_HANDLE;
    VkQueue m_graphicsQueue;
    VkInstance m_instance{};
    VkSurfaceKHR m_surface{};
    VkPhysicalDevice m_physicalDevice{};
    VkDevice m_device{};
};

ELIX_NAMESPACE_END

#endif //VULKAN_CONTEXT_HPP