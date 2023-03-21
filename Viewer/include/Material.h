#pragma once
#include <glm/glm.hpp>

typedef struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
} Material;
