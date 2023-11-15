#pragma once
#include "glm/vec3.hpp"

struct DirectionalLight
{
    glm::vec3 Direction = glm::vec3(-0.2f, -1.0f, -0.3f);

    glm::vec3 Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 Diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    glm::vec3 Specular = glm::vec3(0.5f, 0.5f, 0.5f);
};