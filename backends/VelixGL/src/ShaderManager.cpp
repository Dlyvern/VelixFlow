#include "VelixGL/ShaderManager.hpp"
#include "EmbeddedShaders.hpp"
#include "VelixGL/GLShader.hpp"
#include <iostream>
#include "VelixFlow/Logger.hpp"

ShaderManager& ShaderManager::instance()
{
    static ShaderManager instance;
    return instance;
}

elix::IShader* ShaderManager::getShader(const ShaderType& type)
{
    return m_shaders[type].get();
}

void ShaderManager::preLoadShaders()
{
    auto createShader = [](const char* vert, const char* frag, const std::string& name = "")
    {
        auto shader = std::make_unique<elix::GLShader>();

        if(!shader->loadBinaries(vert, frag))
        {
            ELIX_LOG_ERROR("Failed to load ", name);
        }
        return shader;
    };

    // elix::VKShader testShader;

    // testShader.loadBinaries(nullptr, nullptr);

    m_shaders[SKELETON] = createShader(shader_skeleton_vert, shader_default_frag, "skeleton");
    m_shaders[STATIC] = createShader(shader_cube_vert, shader_default_frag, "static");
    m_shaders[STATIC_SHADOW] = createShader(shader_shadow_vert, shader_empty_frag, "static_shadow");
    m_shaders[SKELETON_SHADOW] = createShader(shader_skeleton_shadow_vert, shader_empty_frag, "skeleton_shadow");
    m_shaders[LINE] = createShader(shader_line_vert, shader_line_frag, "line");
    m_shaders[TEXT] = createShader(shader_text_vert, shader_text_frag, "text");
    m_shaders[STATIC_STENCIL] = createShader(shader_cube_vert, shader_stencil_frag, "static_stencil");
    m_shaders[SKELETON_STENCIL] = createShader(shader_skeleton_vert, shader_stencil_frag, "skeleton_stencil");
    m_shaders[SKYBOX] = createShader(shader_skybox_vert, shader_skybox_frag, "skybox");
    m_shaders[EQUIRECTANGULAR_TO_CUBEMAP] = createShader(shader_equirectangular_to_cubemap_vert, shader_equirectangular_to_cubemap_frag, "EQUIRECTANGULAR_TO_CUBEMAP");
    m_shaders[PARTICLE] = createShader(shader_particle_vert, shader_particle_frag, "particle");
    m_shaders[BILLBOARD] = createShader(shader_billboard_vert, shader_billboard_frag, "billboard");
    m_shaders[UI] = createShader(shader_ui_vert, shader_ui_frag, "ui");
}
