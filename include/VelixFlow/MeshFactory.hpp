#ifndef MESH_FACTORY_HPP
#define MESH_FACTORY_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/Interface/IRenderContext.hpp"
#include "VelixFlow/RenderAPI/Interface/IMesh.hpp"
#include <memory>

ELIX_NAMESPACE_BEGIN

namespace mesh
{
    class MeshFactory
    {
    public:
        static void init(IRenderContext* renderContext);
        static std::shared_ptr<IMesh> createMesh(const MeshData& meshData);
        IRenderContext* getRenderContext();
    private:
        static std::unique_ptr<MeshFactory> s_instance;
        IRenderContext* m_renderContext;
    };
} //namespace mesh

ELIX_NAMESPACE_END

#endif //MESH_FACTORY_HPP