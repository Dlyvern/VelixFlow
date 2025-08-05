#ifndef IRENDER_CONTEXT_HPP
#define IRENDER_CONTEXT_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/Interface/IMesh.hpp"
#include "VelixFlow/RenderAPI/Interface/ITexture.hpp"

#include <memory>

ELIX_NAMESPACE_BEGIN

class IRenderContext
{
public:
    virtual std::shared_ptr<mesh::IMesh> createMesh(const mesh::MeshData& meshData) = 0;
    virtual std::unique_ptr<texture::ITexture> createTexture(const std::string& path) = 0;
    virtual ~IRenderContext() = default;
};

ELIX_NAMESPACE_END

#endif //IRENDER_CONTEXT_HPP