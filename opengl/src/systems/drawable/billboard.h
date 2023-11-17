#pragma once
#include <glm/glm.hpp>

#include "../rendering/texture_component.h"
#include "../shader/shader_component.h"

class Billboard
{
public:
    unsigned int VBO;
    ShaderComponent shader;
    TextureComponent texture;

    Billboard(const ShaderComponent& shader, const TextureComponent& texture) :
    shader(shader), texture(texture)
    {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Setup vertex attribute pointers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Unbind VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    
    void Draw(const glm::mat4& view, const glm::mat4& projection,
              const glm::vec2& size, const glm::vec3& position,
              const glm::vec3& rightWorld, const glm::vec3& upWorld)
    {
        shader.Use()
        .SetMat4("View", view)
        .SetMat4("Projection", projection)
        .SetVec3("BillboardPos", position)
        .SetVec2("BillboardSize", size)
        .SetVec3("CamRightWorld", rightWorld)
        .SetVec3("CamUpWorld", upWorld);

        texture.Bind(0);
        shader.SetInt("texture1", 0);

        // Draw
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // Unbind VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

private:
    inline static float m_vertices[] = { 
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
   };
};
