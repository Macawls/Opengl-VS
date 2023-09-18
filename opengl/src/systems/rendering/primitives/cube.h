#pragma once

#include "../../rendering/transform.h"
#include "../../rendering/perspective_camera.h"
#include "../../shader/shader.h"


class Cube
{
public:
	Cube(Shader shader, glm::vec3 color);

    void Draw(const PerspectiveCamera& camera);
    void Draw(const PerspectiveCamera& camera, glm::vec3 color);
    
    TransformComponent Transform = TransformComponent();
   
private:
    unsigned int m_VAO, m_VBO, m_EBO;

    Shader m_shader;
    glm::vec3 m_color;
    glm::mat4 get_mvp(const PerspectiveCamera& camera);
	
    void init();

    const float m_vertices[36] = {
        // Front face
         0.5,  0.5,  0.5,
        -0.5,  0.5,  0.5,
        -0.5, -0.5,  0.5,
         0.5, -0.5,  0.5,

         // Back face
         0.5,  0.5, -0.5,
        -0.5,  0.5, -0.5,
        -0.5, -0.5, -0.5,
         0.5, -0.5, -0.5,
    };

    const unsigned int m_indices[36] = {
        // Front
        0, 1, 2,
        2, 3, 0,

        // Right
        0, 3, 7,
        7, 4, 0,

        // Bottom
        2, 6, 7,
        7, 3, 2,

        // Left
        1, 5, 6,
        6, 2, 1,

        // Back
        4, 7, 6,
        6, 5, 4,

        // Top
        5, 1, 0,
        0, 4, 5,
    };
};

