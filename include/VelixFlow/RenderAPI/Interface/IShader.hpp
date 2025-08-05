#ifndef ISHADER_HPP
#define ISHADER_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include <string>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

ELIX_NAMESPACE_BEGIN

class IShader
{
public:
    virtual void load(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = {}) = 0;
    virtual bool loadBinaries(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr) = 0;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void setMat4Array(const std::string& name, const std::vector<glm::mat4>& value) const = 0;
    virtual void setMat4(const std::string& name, const glm::mat4& value) const = 0;
    virtual void setVec2(const std::string& name, const glm::vec2& value) const = 0;
    virtual void setVec3(const std::string& name, const glm::vec3& value) const = 0;
    virtual void setVec4(const std::string& name, const glm::vec4& value) const = 0;
    virtual void setFloat(const std::string& name, float value) const = 0;
    virtual void setInt(const std::string& name, int value) const = 0;

    virtual ~IShader() = default;
};

ELIX_NAMESPACE_END

#endif //ISHADER_HPP