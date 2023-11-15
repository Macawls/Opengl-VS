#pragma once
#include "glm/vec3.hpp"

struct SpotLight
{
    glm::vec3 Position;
    glm::vec3 Direction;
    float CutOff = glm::cos(glm::radians(12.5f));
    float OuterCutOff = glm::cos(glm::radians(15.0f));
  
    float Constant = 1.0f;
    float Linear = 0.09f;
    float Quadratic = 0.032f;
  
    glm::vec3 Ambient = glm::vec3(0.0f);
    glm::vec3 Diffuse = glm::vec3(1.0f);
    glm::vec3 Specular = glm::vec3(1.0f);      
};
