#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP

#include <unordered_map>
#include <memory>

#include "VelixFlow/RenderAPI/Interface/IShader.hpp"

class ShaderManager
{
public:
    enum ShaderType : uint16_t
    {
        SKELETON = 0,
        STATIC = 1,
        STATIC_SHADOW = 2,
        SKELETON_SHADOW = 3,
        POST_PROCESSING = 4,
        LINE = 5,
        TEXT = 6,
        STATIC_STENCIL = 7,
        SKELETON_STENCIL = 8,
        SKYBOX = 9,
        EQUIRECTANGULAR_TO_CUBEMAP = 10,
        SHADOW_DEBUG = 11,
        PARTICLE = 12,
        BILLBOARD = 13,
        UI = 14
    };

    static ShaderManager& instance();

    elix::IShader* getShader(const ShaderType& type);

    void preLoadShaders();

private:
    std::unordered_map<ShaderType, std::unique_ptr<elix::IShader>> m_shaders;
};

#endif //SHADER_MANAGER_HPP
