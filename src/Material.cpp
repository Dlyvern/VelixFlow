#include "VelixFlow/Material.hpp"
#include <iostream>

ELIX_NAMESPACE_BEGIN

Material::Material() = default;

Material::Material(const std::string &name) : m_name(name){}

std::string Material::getName() const
{
    return m_name;
}

void Material::setName(const std::string &name)
{
    m_name = name;
}

void Material::setBaseColor(const glm::vec3 &color)
{
    m_baseColor = color;
}

void Material::addTexture(const elix::texture::TextureType &type, texture::ITexture* texture)
{
    m_textures[type] = texture;

    if (type == elix::texture::TextureType::Diffuse)
        m_useBaseColor = texture == nullptr;
}

texture::ITexture* Material::getTexture(const elix::texture::TextureType &type)
{
    return m_textures[type];
}

const std::unordered_map<elix::texture::TextureType, texture::ITexture*>& Material::getTextures() const
{
    return m_textures;
}

const glm::vec3& Material::getBaseColor() const
{
    return m_baseColor;
}

void Material::bind(const IShader* shader)
{
    int textureUnit = 0;

    auto bindTex = [&shader, &textureUnit, this](elix::texture::TextureType type, const std::string& uniformName, const std::string& enabledFlag)
    {
        auto texture = getTexture(type);

        shader->setInt(enabledFlag, texture != nullptr);

        if (texture)
        {
            if (!texture->isUploaded())
            {
                texture->addDefaultParameters();
                texture->upload();
            }


            texture->bind(textureUnit);
            shader->setInt(uniformName, textureUnit);
            textureUnit++;
        }
    };

    bindTex(elix::texture::TextureType::Diffuse,   "u_Diffuse",   "use_Diffuse");
    bindTex(elix::texture::TextureType::Normal,    "u_Normal",    "use_Normal");
    bindTex(elix::texture::TextureType::Metallic,  "u_Metallic",  "use_Metallic");
    bindTex(elix::texture::TextureType::Roughness, "u_Roughness", "use_Roughness");
    bindTex(elix::texture::TextureType::AO,        "u_AO",        "use_AO");

    shader->setVec3("baseColor", m_baseColor);
}

std::shared_ptr<Material> Material::getDefaultMaterial()
{
    if (!m_defaultMaterial)
    {
        m_defaultMaterial = std::make_shared<Material>();
        m_defaultMaterial->setName("default_material");
        m_defaultMaterial->setBaseColor({128, 128, 128});
    }
    
    return m_defaultMaterial;
}

ELIX_NAMESPACE_END