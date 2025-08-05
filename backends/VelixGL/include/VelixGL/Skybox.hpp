#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <vector>
#include <string>

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixGL/GLBuffer.hpp"
#include "VelixGL/VertexArray.hpp"

#include "glm/mat4x4.hpp"

ELIX_NAMESPACE_BEGIN

class Skybox
{
public:
    Skybox();

    void loadFromFaces(const std::vector<std::string>& faces);

    void render(const glm::mat4& view, const glm::mat4& projection) const;

    void loadFromHDR(const std::string& path);

    std::string getAssetPath() const;

private:
    static constexpr unsigned int CUBE_MAP_SIZE{2048};
    unsigned int m_cubeMapTextureId;

    VertexArray m_vertexArray;

    std::string m_assetPath;

    GLBuffer m_vbo{GLBuffer::BufferType::Vertex, GLBuffer::BufferUsage::StaticDraw};

    static unsigned int loadCubemap(const std::vector<std::string>& faces);
};

ELIX_NAMESPACE_END

#endif //SKYBOX_HPP