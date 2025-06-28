#ifndef MESH_COMPONENT_HPP
#define MESH_COMPONENT_HPP

#include "Component.hpp"
#include "Model.hpp"
#include "Material.hpp"
#include <unordered_map>
#include "ShaderManager.hpp"

class MeshComponent final : public Component
{
public:
    explicit MeshComponent(elix::Model* model) : m_model(model)
    {
        for(size_t i = 0; i < model->getNumMeshes(); ++i)
            m_materialOverrides[i] = Material::getDefaultMaterial().get();
    }

    Material* getMaterialOverride(int submeshIndex) const 
    {
        if (m_materialOverrides.contains(submeshIndex))
            return m_materialOverrides.at(submeshIndex);

        return nullptr;
    }
    
    void setMaterialOverride(int submeshIndex, Material* material)
    {
        m_materialOverrides[submeshIndex] = material;
    }

    void render(bool withMaterials = false)
    {
        if(!withMaterials)
            return m_model->draw();
        
        const auto shader = ShaderManager::instance().getShader( m_model->hasSkeleton() ? ShaderManager::ShaderType::SKELETON : ShaderManager::ShaderType::STATIC);

        for (int meshIndex = 0; meshIndex < m_model->getNumMeshes(); meshIndex++)
        {
            auto* mesh = m_model->getMesh(meshIndex);

            Material* material{nullptr};
            
            if (m_materialOverrides.contains(meshIndex))
                material = m_materialOverrides[meshIndex];

            if (!material)
                continue;

            material->bind(*shader);

            mesh->draw();
        }
    }

    void update(float deltaTime) override {}

    [[nodiscard]] elix::Model* getModel() const {return m_model;}
private:
    elix::Model* m_model{nullptr};

    std::unordered_map<int, Material*> m_materialOverrides;
};

#endif //MESH_COMPONENT_HPP
