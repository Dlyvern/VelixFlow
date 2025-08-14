#ifndef GL_SHADER_HPP
#define GL_SHADER_HPP

#include "VelixFlow/RenderAPI/Interface/IShader.hpp"
#include "VelixFlow/DefaultMacros.hpp"
#include <unordered_map>
#include <cstdint>

//TODO maybe we do need shader namespace

ELIX_NAMESPACE_BEGIN

class GLShader : public IShader
{
public:
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
    int m_id{0};
    mutable std::unordered_map<std::string, int> m_uniformCache;

    int getUniformLocation(const std::string& name) const;
};

ELIX_NAMESPACE_END

#endif //GL_SHADER_HPP