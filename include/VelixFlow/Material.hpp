#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <memory>

#include <unordered_map>

#include "VelixFlow/RenderAPI/Interface/ITexture.hpp"
#include "VelixFlow/RenderAPI/Interface/IShader.hpp"

ELIX_NAMESPACE_BEGIN

class Material
{
public:
    Material();

    explicit Material(const std::string& name);

    std::string getName() const;

    void setName(const std::string& name);

    void setBaseColor(const glm::vec3& color);

    void addTexture(const elix::texture::TextureType& type, texture::ITexture* texture);

    texture::ITexture* getTexture(const elix::texture::TextureType& type);
    const std::unordered_map<elix::texture::TextureType, texture::ITexture*>& getTextures() const;

    const glm::vec3& getBaseColor() const;

    void bind(const IShader* shader);

    static std::shared_ptr<Material> getDefaultMaterial();
private:
    static inline std::shared_ptr<Material> m_defaultMaterial{nullptr};

    std::string m_name{"Undefined"};
    std::unordered_map<elix::texture::TextureType, texture::ITexture*> m_textures;
    glm::vec3 m_baseColor = glm::vec3(128, 128, 128);
    bool m_useBaseColor = true;
};

ELIX_NAMESPACE_END

#endif //MATERIAL_HPP
