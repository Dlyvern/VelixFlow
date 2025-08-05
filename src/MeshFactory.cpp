#include "VelixFlow/MeshFactory.hpp"

ELIX_NAMESPACE_BEGIN

namespace mesh
{
    std::unique_ptr<MeshFactory> MeshFactory::s_instance;
    
    void MeshFactory::init(IRenderContext* renderContext)
    {
        if(s_instance)
        {
            s_instance.release();
            s_instance = nullptr;
        }

        s_instance = std::make_unique<MeshFactory>();

        s_instance->m_renderContext = renderContext;
    }

    IRenderContext* MeshFactory::getRenderContext()
    {
        return m_renderContext;
    }

    std::shared_ptr<IMesh> MeshFactory::createMesh(const MeshData& meshData)
    {
        if(!s_instance)
            return nullptr;

        return s_instance->m_renderContext->createMesh(meshData);
    }

} //namespace mesh

ELIX_NAMESPACE_END