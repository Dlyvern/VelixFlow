#ifndef VK_SHADER_HPP
#define VK_SHADER_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/Interface/IShader.hpp"

#include "VelixFlow/RenderAPI/Interface/IRenderContext.hpp"
#include <cstdint>

#include "vulkan/vulkan.h"

ELIX_NAMESPACE_BEGIN

class VKShader : public IShader
{
public:
    bool create(const std::vector<char>& code, IRenderContext* renderContext);

    void load(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = {}) override;
    bool loadBinaries(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr) override;

    void bind() const override;
    void unbind() const override;

    void setMat4Array(const std::string& name, const std::vector<glm::mat4>& value) const override;
    void setMat4(const std::string& name, const glm::mat4& value) const override;
    void setVec2(const std::string& name, const glm::vec2& value) const override;
    void setVec3(const std::string& name, const glm::vec3& value) const override;
    void setVec4(const std::string& name, const glm::vec4& value) const override;
    void setFloat(const std::string& name, float value) const override;
    void setInt(const std::string& name, int value) const override;
private:

    VkShaderModule createShaderModule(const std::vector<char>& code, IRenderContext* renderContext);

    VkShaderModule m_vertexShaderModule{};
    VkShaderModule m_fragmentShaderModule{};
    VkDevice m_device{};
};

ELIX_NAMESPACE_END

#endif //VK_SHADER_HPP