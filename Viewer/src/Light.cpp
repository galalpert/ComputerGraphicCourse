#include "Light.h"

Light::Light(int id, glm::vec3 position) :
    name("light" + std::to_string(id)),
    position(position),
    ambient(glm::vec3(1.0f, 1.0f, 1.0f)),
    diffuse(glm::vec3(1.0f, 1.0f, 1.0f)),
    specular(glm::vec3(0.1f, 0.1f, 0.1f))
{
}

std::string Light::GetName()
{
    return name;
    
}
glm::vec3& Light::GetPosition()
{
    return position;
}
