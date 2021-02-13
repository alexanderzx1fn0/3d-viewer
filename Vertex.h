#ifndef VERTEX_H_
#define VERTEX_H_

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec2 texcoord;
};

#endif
