#ifndef MESH_COMPONENT_HPP
#define MESH_COMPONENT_HPP

#include "VelixFlow/Model.hpp"
#include "VelixFlow/Material.hpp"
#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/Components/Component.hpp"

#include <unordered_map>

ELIX_NAMESPACE_BEGIN

namespace components
{
    class MeshComponent final : public IComponent
    {
    public:
        explicit MeshComponent(Model* model);

        Material* getMaterialOverride(int submeshIndex) const;

        void setMaterialOverride(int submeshIndex, Material* material);

        void update(float deltaTime) override {}

        [[nodiscard]] Model* getModel() const;
    private:
        Model* m_model{nullptr};
        std::unordered_map<int, Material*> m_materialOverrides;
    };

} //namespace components

ELIX_NAMESPACE_END

#endif //MESH_COMPONENT_HPP
