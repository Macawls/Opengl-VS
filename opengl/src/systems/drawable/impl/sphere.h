#pragma once

#include "../drawable.h"

class Sphere final : public Drawable
{
public:
    using Drawable::Drawable;
    Sphere(const ShaderComponent& shader, const glm::vec3& color);
    Sphere(const ShaderComponent& shader, const glm::vec3& color, const TextureComponent& texture);
    void Construct();
};

inline Sphere::Sphere(const ShaderComponent& shader, const glm::vec3& color): Drawable(shader, color)
{
    Construct();
}

inline Sphere::Sphere(const ShaderComponent& shader, const glm::vec3& color, const TextureComponent& texture): Drawable(shader, color, texture)
{
    Construct();
}

inline void Sphere::Construct()
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

            auto vertex = Vertex
            {
                glm::vec3(x, y, z), // pos
                normalize(glm::vec3(x, y, z)), // normal
                glm::vec2(U, V) // tex
            };

            m_vertices.push_back(vertex);
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

    Init();
}



