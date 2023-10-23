#pragma once

#include "../drawable.h"

class Sphere : public Drawable
{
public:
    using Drawable::Drawable;
    Sphere(const Shader& shader, const glm::vec3& color);
    Sphere(const Shader& shader, const glm::vec3& color, const Texture& texture);
};

inline Sphere::Sphere(const Shader& shader, const glm::vec3& color): Drawable(shader, color)
{
    Transform.GuiDisplay = "Sphere";
    
    const int stacks = SPHERE_GEN.Stacks;
    const float radius = SPHERE_GEN.Radius;
    const int sectors = SPHERE_GEN.Sectors;

    // gen vertices
    for (int i = 0; i <= stacks; ++i)
    {
        const float V = i / (float)stacks;
        const float phi = V * glm::pi<float>();

        for (int j = 0; j <= sectors; ++j)
        {
            const float U = j / (float)sectors;
            const float theta = U * (glm::pi<float>() * 2);

            // Calc The Vertex Positions
            float x = radius * cosf(theta) * sinf(phi);
            float y = radius * cosf(phi);
            float z = radius * sinf(theta) * sinf(phi);


            m_vertices.push_back(x);
            m_vertices.push_back(y);
            m_vertices.push_back(z);
        }
    }

    // gen indices
    for (int i = 0; i < sectors * stacks + sectors; ++i)
    {

        m_indices.push_back(i);
        m_indices.push_back(i + sectors + 1);
        m_indices.push_back(i + sectors);

        m_indices.push_back(i + sectors + 1);
        m_indices.push_back(i);
        m_indices.push_back(i + 1);
    }

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

inline Sphere::Sphere(const Shader& shader, const glm::vec3& color, const Texture& texture) : Drawable(shader, color, texture)
{
    Transform.GuiDisplay = "Sphere";
    
    const int stacks = SPHERE_GEN.Stacks;
    const float radius = SPHERE_GEN.Radius;
    const int sectors = SPHERE_GEN.Sectors;

    // Generate vertices and texture coordinates
    for (int i = 0; i <= stacks; ++i)
    {
        const float V = i / static_cast<float>(stacks);
        const float phi = V * glm::pi<float>();

        for (int j = 0; j <= sectors; ++j)
        {
            const float U = j / static_cast<float>(sectors);
            const float theta = U * (glm::pi<float>() * 2);

            // Calculate the Vertex Positions
            float x = radius * cosf(theta) * sinf(phi);
            float y = radius * cosf(phi);
            float z = radius * sinf(theta) * sinf(phi);

            m_vertices.push_back(x);
            m_vertices.push_back(y);
            m_vertices.push_back(z);
            
            // Calculate and store the texture coordinates
            float texU = 1.0f - U;
            float texV = 1.0f - V;
            m_textureCoords.push_back(texU);
            m_textureCoords.push_back(texV);
        }
    }

    // Generate indices
    for (int i = 0; i < sectors * stacks + sectors; ++i)
    {
        m_indices.push_back(i);
        m_indices.push_back(i + sectors + 1);
        m_indices.push_back(i + sectors);

        m_indices.push_back(i + sectors + 1);
        m_indices.push_back(i);
        m_indices.push_back(i + 1);
    }

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    // Set the vertex attribute pointers for position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set the vertex attribute pointers for texture coordinates
    glGenBuffers(1, &m_textureVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_textureVbo);
    glBufferData(GL_ARRAY_BUFFER, m_textureCoords.size() * sizeof(float), m_textureCoords.data(), GL_STATIC_DRAW);

    // Attribute pointer at 1 for texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

