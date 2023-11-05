#pragma once

#include "../rendering/transform.h"
#include "../rendering/vertex.h"
#include "../rendering/perspective_camera.h"
#include "../shader/shader_component.h"
#include "../rendering/texture_component.h"
#include "gen_data.h"

class Drawable {
public:
    Drawable(const ShaderComponent& shader, const glm::vec3& color)
        : m_vao(0), m_vbo(0), m_ebo(0), m_textureVbo(0), Color(color), Shader(shader)
    {
        Transform.GuiDisplay = "Drawable";
    }

    Drawable(const ShaderComponent& shader)
        : m_vao(0), m_vbo(0), m_ebo(0), m_textureVbo(0), Shader(shader)
    {
        Transform.GuiDisplay = "Drawable";
    }

    Drawable(const ShaderComponent& shader, const glm::vec3& color, const TextureComponent& texture)
    : m_vao(0), m_vbo(0), m_ebo(0), m_textureVbo(0), Color(color), Shader(shader), Texture(texture), HasTexture(true)
    {
        Transform.GuiDisplay = "Drawable";
    }
    
    virtual void Draw(PerspectiveCamera& camera);
    virtual void Draw(PerspectiveCamera& camera, const glm::vec3& color);
    virtual ~Drawable() = default;

    TransformComponent Transform;
    TextureComponent Texture;
    ShaderComponent Shader;
    glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f);
    bool HasTexture;

protected:
    unsigned int m_vao, m_vbo, m_ebo, m_textureVbo;
    std::vector<unsigned int> m_indices;
    std::vector<Vertex> m_vertices;
    
    void Init();
};

inline void Drawable::Draw(PerspectiveCamera& camera)
{
    // Set shader in opengl state machine
    Shader.Use()
        .SetMat4("mvp", camera.GetProjectionMatrix() * camera.GetViewMatrix() * Transform.GetModelMatrix())
        .SetVec3("color", Color);


    Texture.Bind(0);
    Shader.SetInt("texture1", 0);
    
    
    // Draw 
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind
    glBindVertexArray(0);


    Texture.Unbind();
}

inline void Drawable::Draw(PerspectiveCamera& camera, const glm::vec3& color)
{
    // Set shader in opengl state machine
    Shader.Use()
        .SetMat4("mvp", camera.GetProjectionMatrix() * camera.GetViewMatrix() * Transform.GetModelMatrix())
        .SetVec3("color", color);
    

    Texture.Bind(0);
    Shader.SetInt("texture1", 0);

   
    
    // Draw 
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind
    glBindVertexArray(0);


    Texture.Unbind();
}

// not called for terrain
inline void Drawable::Init()
{
    // Create the VAO, VBO, and EBO
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    // Bind the VAO
    glBindVertexArray(m_vao);

    // Bind and buffer data into VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    // Bind and buffer data into EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    // Set the vertex attribute pointers for position, normal, and texture coordinates
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    // Unbind VAO
    glBindVertexArray(0);
}


