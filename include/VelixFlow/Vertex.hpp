#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "DefaultMacros.hpp"
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

ELIX_NAMESPACE_BEGIN

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoordinates{0.0f, 0.0f};
    glm::vec3 tangent{0.0f, 0.0f, 0.0f};
    glm::vec3 bitangent{0.0f, 0.0f, 0.0f};
    glm::ivec4 boneID = glm::ivec4(-1);
    glm::vec4 weight = glm::vec4(0);
};


ELIX_NAMESPACE_END

#endif //VERTEX_HPP