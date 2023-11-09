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
        : Vao(0), Vbo(0), Ebo(0), Color(color), Shader(shader)
    {
        Transform.GuiDisplay = "Drawable";
    }

    Drawable(const ShaderComponent& shader)
        : Vao(0), Vbo(0), Ebo(0), Shader(shader)
    {
        Transform.GuiDisplay = "Drawable";
    }

    Drawable(const ShaderComponent& shader, const glm::vec3& color, const TextureComponent& texture)
    : Vao(0), Vbo(0), Ebo(0), Color(color), Shader(shader), Texture(texture)
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

    unsigned int Vao, Vbo, Ebo;
    std::vector<unsigned int> Indices;
    std::vector<Vertex> Vertices;
protected:
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
    glBindVertexArray(Vao);
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

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
    glBindVertexArray(Vao);
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind
    glBindVertexArray(0);
    Texture.Unbind();
}

// not called for terrain/cubemap
inline void Drawable::Init()
{
    // Create the VAO, VBO, and EBO
    glGenVertexArrays(1, &Vao);
    glGenBuffers(1, &Vbo);
    glGenBuffers(1, &Ebo);

    // Bind the VAO
    glBindVertexArray(Vao);

    // Bind and buffer data into VBO
    glBindBuffer(GL_ARRAY_BUFFER, Vbo);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

    // Bind and buffer data into EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), Indices.data(), GL_STATIC_DRAW);

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


