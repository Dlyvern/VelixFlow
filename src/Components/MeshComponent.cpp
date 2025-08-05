#include "VelixFlow/Components/MeshComponent.hpp"

ELIX_NAMESPACE_BEGIN

namespace components
{
    MeshComponent::MeshComponent(Model* model) : m_model(model)
    {
        for(size_t i = 0; i < model->getNumMeshes(); ++i)
            m_materialOverrides[i] = Material::getDefaultMaterial().get();
    }

    Material* MeshComponent::getMaterialOverride(int submeshIndex) const 
    {
        if (m_materialOverrides.contains(submeshIndex))
            return m_materialOverrides.at(submeshIndex);

        return nullptr;
    }

    void MeshComponent::setMaterialOverride(int submeshIndex, Material* material)
    {
        m_materialOverrides[submeshIndex] = material;
    }

    Model* MeshComponent::getModel() const
    {
        return m_model;
    }

} //namespace components

ELIX_NAMESPACE_END