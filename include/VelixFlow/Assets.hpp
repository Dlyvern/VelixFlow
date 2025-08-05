#ifndef ASSETS_HPP
#define ASSETS_HPP

#include "VelixFlow/RenderAPI/Interface/ITexture.hpp"
#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/Model.hpp"
#include "VelixFlow/Animation.hpp"
#include "VelixFlow/Material.hpp"

#include <memory>

ELIX_NAMESPACE_BEGIN

class Asset
{
public:
    virtual ~Asset() = default;
};

class AssetModel final : public Asset
{
public:
    explicit AssetModel(std::unique_ptr<Model> model) : m_model(std::move(model)) {}
    [[nodiscard]] Model* getModel() const { return m_model.get(); }
private:
    std::unique_ptr<Model> m_model{nullptr};
};

class AssetMaterial final : public Asset
{
public:
    explicit AssetMaterial(std::unique_ptr<Material> material) : m_material(std::move(material)) {}
    [[nodiscard]] Material* getMaterial() const { return m_material.get(); }
private:
    std::unique_ptr<Material> m_material{nullptr};
};

class AssetTexture final : public Asset
{
public:
    explicit AssetTexture(std::unique_ptr<texture::ITexture> texture) : m_texture(std::move(texture)) {}

    [[nodiscard]] texture::ITexture* getTexture() const { return m_texture.get(); }
private:
    std::unique_ptr<texture::ITexture> m_texture{nullptr};
};

class AssetAnimation final : public Asset
{
public:
    explicit AssetAnimation(std::unique_ptr<animation::Animation> animation) : m_animation(std::move(animation)) {}

    [[nodiscard]] animation::Animation* getAnimation() const { return m_animation.get(); }

private:
    std::unique_ptr<animation::Animation> m_animation{nullptr};
};

ELIX_NAMESPACE_END

#endif //ASSETS_HPP
