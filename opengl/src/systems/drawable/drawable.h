#pragma once

#include "../rendering/transform.h"
#include "../rendering/perspective_camera.h"
#include "../shader/shader.h"
#include "../shader/shader.h"

#include "gen_data.h"
#include "../rendering/texture.h"

class Drawable {
public:
    Drawable(const Shader& shader, const glm::vec3& color)
        : m_vao(0), m_vbo(0), m_ebo(0), m_textureVbo(0), hasTexture(false), m_color(color), m_shader(shader)
    {
        Transform.GuiDisplay = "Drawable";
    }

    Drawable(const Shader& shader, const glm::vec3& color, const Texture& texture)
        : m_vao(0), m_vbo(0), m_ebo(0), m_textureVbo(0), hasTexture(true),
        m_color(color), m_shader(shader), m_texture(texture)
    {
        Transform.GuiDisplay = "Drawable";
    }

    Drawable(const Shader& shader)
        : m_vao(0), m_vbo(0), m_ebo(0), m_textureVbo(0), hasTexture(false), m_shader(shader)
    {
        Transform.GuiDisplay = "Drawable";
    }

    virtual void Draw(PerspectiveCamera& camera);
    virtual void Draw(PerspectiveCamera& camera, const glm::vec3& color);
    virtual ~Drawable() = default;
    TransformComponent Transform;

protected:
    unsigned int m_vao, m_vbo, m_ebo, m_textureVbo;
    std::vector<float> m_vertices;
    std::vector<float> m_textureCoords;
    std::vector<unsigned int> m_indices;
    bool hasTexture;
    

    glm::vec3 m_color = glm::vec3(1.0f, 1.0f, 1.0f);
    Shader m_shader;
    Texture m_texture;
};

inline void Drawable::Draw(PerspectiveCamera& camera)
{
    // Set shader in opengl state machine
    m_shader.Use()
        .SetMat4("mvp", camera.GetProjectionMatrix() * camera.GetViewMatrix() * Transform.GetModelMatrix())
        .SetVec3("color", m_color);

    
    if (hasTexture)
    {
        m_texture.Bind(0);
        m_shader.SetInt("texture1", 0);
    }
    

    // Draw 
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind
    glBindVertexArray(0);

    
    if (hasTexture)
    {
        m_texture.Unbind();
    }
    
}

inline void Drawable::Draw(PerspectiveCamera& camera, const glm::vec3& color)
{
    // Set shader in opengl state machine
    m_shader.Use()
        .SetMat4("mvp", camera.GetProjectionMatrix() * camera.GetViewMatrix() * Transform.GetModelMatrix())
        .SetVec3("color", color);
    
    if (hasTexture)
    {
        m_texture.Bind(0);
        m_shader.SetInt("texture1", 0);
    }
    
    
    // Draw 
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind
    glBindVertexArray(0);
    
    if (hasTexture)
    {
        m_texture.Unbind();
    }
    
}


