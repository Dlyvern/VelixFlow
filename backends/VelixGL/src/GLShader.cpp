#include "VelixGL/GLShader.hpp"
#include "VelixFlow/Logger.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "glad/glad.h"

namespace
{
    std::string readFile(const std::string& filePath)
    {
        std::ifstream file(filePath);

        if (!file.is_open())
            return {};

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    int checkCompileErrors(const unsigned int shader, const char* type)
    {
        int success;
        char infoLog[1024];

        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                ELIX_LOG_ERROR("Shader compilation error of type: ", type, " ", infoLog);
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);

            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                ELIX_LOG_ERROR("Shader linking error of type: ", type, " ", infoLog);
            }
        }

        return success;
    }
}


ELIX_NAMESPACE_BEGIN

void GLShader::load(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{
if (m_id)
    {
        glDeleteProgram(m_id);
        m_uniformCache.clear();
    }

    const std::string vertexSource = ::readFile(vertexPath);
    const std::string fragmentSource = ::readFile(fragmentPath);

    const char* vertexCode = vertexSource.c_str();
    const char* fragmentCode = fragmentSource.c_str();

    const GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, nullptr);
    glCompileShader(vertex);
   
    if(::checkCompileErrors(vertex, "VERTEX"))
	    ELIX_LOG_ERROR("Failed to compile: ", vertexPath);

    const GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, nullptr);
    glCompileShader(fragment);

    if(::checkCompileErrors(fragment, "FRAGMENT"))
	    ELIX_LOG_ERROR("Failed to compile: ", fragmentPath);

    GLuint geometry = 0;

    if (!geometryPath.empty())
    {
        const std::string geometrySource = ::readFile(geometryPath);
        const char* geometryCode = geometrySource.c_str();

        geometry = glCreateShader(GL_GEOMETRY_SHADER);

        glShaderSource(geometry, 1, &geometryCode, nullptr);
        glCompileShader(geometry);
        ::checkCompileErrors(geometry, "GEOMETRY");
    }

    const int tempID = glCreateProgram();
    glAttachShader(tempID, vertex);
    glAttachShader(tempID, fragment);

    if (geometry)
        glAttachShader(tempID, geometry);

    glLinkProgram(tempID);

    if (::checkCompileErrors(tempID, "PROGRAM"))
    {
        if (m_id != -1)
            glDeleteProgram(m_id);

        m_id = tempID;
    }
    else
        ELIX_LOG_ERROR("Shader failed to compile ", vertexPath, fragmentPath);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    if (geometry)
        glDeleteShader(geometry);
}

bool GLShader::loadBinaries(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
    if (m_id)
    {
        glDeleteProgram(m_id);
        m_uniformCache.clear();
    }

    bool result = true;

    const GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexPath, nullptr);
    glCompileShader(vertex);

    if(!::checkCompileErrors(vertex, "VERTEX"))
        result = false;

    const GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentPath, nullptr);
    glCompileShader(fragment);

    if(!::checkCompileErrors(fragment, "FRAGMENT"))
        result = false;
    
    GLuint geometry = 0;

    if (geometryPath)
    {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &geometryPath, nullptr);
        glCompileShader(geometry);

        if(!::checkCompileErrors(geometry, "GEOMETRY"))
            result = false;
    }

    const int tempID = glCreateProgram();
    glAttachShader(tempID, vertex);
    glAttachShader(tempID, fragment);

    if (geometry)
        glAttachShader(tempID, geometry);

    glLinkProgram(tempID);

    if (::checkCompileErrors(tempID, "PROGRAM"))
    {
        if (m_id != -1)
            glDeleteProgram(m_id);

        m_id = tempID;
    }
    else
        ELIX_LOG_ERROR("Shader failed to compile embedded sources");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    if (geometry)
        glDeleteShader(geometry);

    return result;
}

void GLShader::bind() const
{
    glUseProgram(m_id);
}

void GLShader::unbind() const
{
    glUseProgram(0);
}

void GLShader::setMat4Array(const std::string& name, const std::vector<glm::mat4>& value) const
{
    if(m_id == 0)
        return;
    
    GLint location = getUniformLocation(name);
    glUniformMatrix4fv(location, value.size(), GL_FALSE, glm::value_ptr(value[0]));
}


void GLShader::setMat4(const std::string& name, const glm::mat4& value) const
{
    if(m_id == 0)
        return;
    const GLint location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void GLShader::setVec2(const std::string& name, const glm::vec2& value) const
{
    if(m_id == 0)
        return;
    const GLint location = getUniformLocation(name);
    glUniform2fv(location, 1, &value[0]);
}

void GLShader::setVec3(const std::string& name, const glm::vec3& value) const
{
    if(m_id == 0)
        return;

    const GLint location = getUniformLocation(name);
    glUniform3fv(location, 1, &value[0]);
}

void GLShader::setVec4(const std::string& name, const glm::vec4& value) const
{
    if(m_id == 0)
        return;

    const GLint location = getUniformLocation(name);
    glUniform4fv(location, 1, &value[0]);
}

void GLShader::setFloat(const std::string& name, float value) const
{
    if(m_id == 0)
        return;

    const GLint location = getUniformLocation(name);
    glUniform1f(location, value);
}

void GLShader::setInt(const std::string& name, int value) const
{
    if(m_id == 0)
        return;

    const GLint location = getUniformLocation(name);
    glUniform1i(location, value);
}

int GLShader::getUniformLocation(const std::string& name) const
{
    if (const auto it = m_uniformCache.find(name); it != m_uniformCache.end())
        return it->second;

    const GLint location = glGetUniformLocation(m_id, name.c_str());

    if (location == -1)
        ELIX_LOG_WARN("Uniform ", name, " not found");

    m_uniformCache[name] = location;

    return location;
}


ELIX_NAMESPACE_END