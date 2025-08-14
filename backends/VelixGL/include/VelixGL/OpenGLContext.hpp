#ifndef OPENGL_CONTEXT_HPP
#define OPENGL_CONTEXT_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/RenderAPI/Interface/IRenderContext.hpp"
#include <cstdint>

ELIX_NAMESPACE_BEGIN

class OpenGLContext : public IRenderContext
{
public:
    void init();

    void cleanup();

    std::shared_ptr<mesh::IMesh> createMesh(const mesh::MeshData& meshData);
    std::unique_ptr<texture::ITexture> createTexture(const std::string& path);

};

ELIX_NAMESPACE_END

#endif //OPENGL_CONTEXT_HPP