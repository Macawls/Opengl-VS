#pragma once
#include "glm/vec3.hpp"

struct PointLight 
{
    TransformComponent Transform;
    
    float Constant = 1.0f;
    float Linear = 0.09f;
    float Quadratic = 0.032f;

    glm::vec3 Ambient = glm::vec3(0.1f);
    glm::vec3 Diffuse = glm::vec3(0.8f);
    glm::vec3 Specular = glm::vec3(1.0f);
};
