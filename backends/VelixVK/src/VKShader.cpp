#include "VelixVK/VulkanContext.hpp"
#include "VelixVK/VKShader.hpp"
#include "VelixFlow/Logger.hpp"

ELIX_NAMESPACE_BEGIN

bool VKShader::loadBinaries(const char* vertexPath, const char* fragmentPath, const char* geometryPath) 
{

    return true;
}

void VKShader::load(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{

}

bool VKShader::create(const std::vector<char>& code, IRenderContext* renderContext)
{
    auto vulkanContext = dynamic_cast<VulkanContext*>(renderContext);

    if(!vulkanContext)
        return false;

    VkShaderModule fragmentShaderModule = createShaderModule(code, renderContext);
    VkShaderModule vertexShaderModule = createShaderModule(code, renderContext);

    VkPipelineShaderStageCreateInfo fragmentCreateInfo{};
    fragmentCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentCreateInfo.module = fragmentShaderModule;
    fragmentCreateInfo.pName = "main";

    VkPipelineShaderStageCreateInfo vertexCreateInfo{};
    vertexCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexCreateInfo.module = vertexShaderModule;

    VkPipelineShaderStageCreateInfo shaderStages[] = {fragmentCreateInfo, vertexCreateInfo};

    

    return true;
}

VkShaderModule VKShader::createShaderModule(const std::vector<char>& code, IRenderContext* renderContext)
{
    VkShaderModuleCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;

    auto vulkanContext = dynamic_cast<VulkanContext*>(renderContext);

    if(vkCreateShaderModule(vulkanContext->getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        ELIX_LOG_ERROR("Failed to create shader module");

    return shaderModule;
}

//TODO We do not need those functions(Make RHI better for OpenGL and Vulkan)
void VKShader::bind() const 
{

}
void VKShader::unbind() const 
{

}

void VKShader::setMat4Array(const std::string& name, const std::vector<glm::mat4>& value) const 
{

}
void VKShader::setMat4(const std::string& name, const glm::mat4& value) const 
{

}
void VKShader::setVec2(const std::string& name, const glm::vec2& value) const 
{

}
void VKShader::setVec3(const std::string& name, const glm::vec3& value) const 
{

}
void VKShader::setVec4(const std::string& name, const glm::vec4& value) const 
{

}
void VKShader::setFloat(const std::string& name, float value) const 
{

}
void VKShader::setInt(const std::string& name, int value) const 
{

}


ELIX_NAMESPACE_END