#pragma once

#include "../drawable.h"

class Cone : public Drawable
{
public:
    using Drawable::Drawable;
    Cone(const Shader& shader, const glm::vec3& color);
    Cone(const Shader& shader, const glm::vec3& color, const Texture& texture);
};

inline Cone::Cone(const Shader& shader, const glm::vec3& color) : Drawable(shader, color)
{
    Transform.GuiDisplay = "Cone";
    
    const float height = CONE_GEN.Height;
    const int sectors = CONE_GEN.Sectors;
    const int stacks = CONE_GEN.Stacks;
    const float bottomRadius = CONE_GEN.BottomRadius;
    const float topRadius = CONE_GEN.TopRadius;

    // generate unit circle vectors on XY-plane
    const float sectorStep = 2 * PI / sectors;
    std::vector<float> unitCircleVertices;
    for (int i = 0; i <= sectors; ++i)
    {
        const float sectorAngle = i * sectorStep;
        unitCircleVertices.push_back(cos(sectorAngle)); // x
        unitCircleVertices.push_back(sin(sectorAngle)); // y
        unitCircleVertices.push_back(0);                // z
    }

    // put side vertices to arrays
    for (int i = 0; i < 2; ++i)
    {
        float h = -height / 2.0f + i * height;           // z value  [-h/2 to h/2]
        float t = 1.0f - i;                              // vertical tex coord  [1 to 0]

        for (int j = 0, k = 0; j <= sectors; ++j, k += 3)
        {
            const float rad = bottomRadius + (float)i / stacks * (topRadius - bottomRadius);      // lerp
            const float ux = unitCircleVertices[k];
            const float uy = unitCircleVertices[k + 1];
            float uz = unitCircleVertices[k + 2];
            // position 
            m_vertices.push_back(ux * rad);             // vx
            m_vertices.push_back(uy * rad);             // vy
            m_vertices.push_back(h);                        // vz
            // normal
            //normals.push_back(ux);                       // nx
            //normals.push_back(uy);                       // ny
            //normals.push_back(uz);                       // nz
            // tex
            //texCoords.push_back((float)j / sectors); // s
            //texCoords.push_back(t);                      // t
        }
    }

    // the starting index for the base/top surface
    //NOTE: it is used for generating indices later
    int baseCenterIndex = (int)m_vertices.size() / 3;
    int topCenterIndex = baseCenterIndex + sectors + 1; // include center vertex

    // put base and top vertices to arrays
    for (int i = 0; i < 2; ++i)
    {
        float h = -height / 2.0f + i * height;              // z value  [-h/2 to h/2]
        float nz = -1 + i * 2;                              // z val normal [-1 to 1]

        // center point
        m_vertices.push_back(0);
        m_vertices.push_back(0);
        m_vertices.push_back(h);
        //normals.push_back(0);      normals.push_back(0);      normals.push_back(nz);
        //texCoords.push_back(0.5f); texCoords.push_back(0.5f);

        for (int j = 0, k = 0; j < sectors; ++j, k += 3)
        {
            const float rad = bottomRadius + (float)i / stacks * (topRadius - bottomRadius);     // lerp
            const float ux = unitCircleVertices[k];
            const float uy = unitCircleVertices[k + 1];
            // position
            // m_vertices.push_back(ux * radius);             // vx
            m_vertices.push_back(ux * rad);             // vx
            // m_vertices.push_back(uy * radius);             // vy
            m_vertices.push_back(uy * rad);             // vy
            m_vertices.push_back(h);                       // vz
            // normal
            //normals.push_back(0);                        // nx
            //normals.push_back(0);                        // ny
            //normals.push_back(nz);                       // nz
            // tex
            //texCoords.push_back(-ux * 0.5f + 0.5f);      // s
            //texCoords.push_back(-uy * 0.5f + 0.5f);      // t
        }
    }


    // indices
    int k1 = 0;                         // 1st vertex index at base
    int k2 = sectors + 1;           // 1st vertex index at top

    // indices for the side surface
    for (int i = 0; i < sectors; ++i, ++k1, ++k2)
    {
        // 2 triangles per sector
        // k1 => k1+1 => k2
        m_indices.push_back(k1);
        m_indices.push_back(k1 + 1);
        m_indices.push_back(k2);

        // k2 => k1+1 => k2+1
        m_indices.push_back(k2);
        m_indices.push_back(k1 + 1);
        m_indices.push_back(k2 + 1);
    }

    // indices for the base
    for (int i = 0, k = baseCenterIndex + 1; i < sectors; ++i, ++k)
    {
        if (i < sectors - 1)
        {
            m_indices.push_back(baseCenterIndex);
            m_indices.push_back(k + 1);
            m_indices.push_back(k);
        }
        else
        {
            // last triangle
            m_indices.push_back(baseCenterIndex);
            m_indices.push_back(baseCenterIndex + 1);
            m_indices.push_back(k);
        }
    }

    // indices for the top 
    for (int i = 0, k = topCenterIndex + 1; i < sectors; ++i, ++k)
    {
        if (i < sectors - 1)
        {
            m_indices.push_back(topCenterIndex);
            m_indices.push_back(k);
            m_indices.push_back(k + 1);
        }
        else
        {
            // last triangle
            m_indices.push_back(topCenterIndex);
            m_indices.push_back(k);
            m_indices.push_back(topCenterIndex + 1);
        }
    }

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
inline Cone::Cone(const Shader& shader, const glm::vec3& color, const Texture& texture) : Drawable(shader, color, texture)
{
    Transform.GuiDisplay = "Cone";
    
    const float height = CONE_GEN.Height;
    const int sectors = CONE_GEN.Sectors;
    const int stacks = CONE_GEN.Stacks;
    const float bottomRadius = CONE_GEN.BottomRadius;
    const float topRadius = CONE_GEN.TopRadius;

    // generate unit circle vectors on XY-plane
    const float sectorStep = 2 * PI / sectors;
    std::vector<float> unitCircleVertices;
    for (int i = 0; i <= sectors; ++i)
    {
        const float sectorAngle = i * sectorStep;
        unitCircleVertices.push_back(cos(sectorAngle)); // x
        unitCircleVertices.push_back(sin(sectorAngle)); // y
        unitCircleVertices.push_back(0);                // z
    }

    // put side vertices to arrays
    for (int i = 0; i < 2; ++i)
    {
        float h = -height / 2.0f + i * height;           // z value  [-h/2 to h/2]
        float t = 1.0f - i;                              // vertical tex coord  [1 to 0]

        for (int j = 0, k = 0; j <= sectors; ++j, k += 3)
        {
            const float rad = bottomRadius + (float)i / stacks * (topRadius - bottomRadius);      // lerp
            const float ux = unitCircleVertices[k];
            const float uy = unitCircleVertices[k + 1];
            float uz = unitCircleVertices[k + 2];
            // position 
            m_vertices.push_back(ux * rad);             // vx
            m_vertices.push_back(uy * rad);             // vy
            m_vertices.push_back(h);                        // vz
            // normal
            //normals.push_back(ux);                       // nx
            //normals.push_back(uy);                       // ny
            //normals.push_back(uz);                       // nz
            // tex
            m_textureCoords.push_back((float)j / sectors); // s
            m_textureCoords.push_back(t);                      // t
        }
    }

    // the starting index for the base/top surface
    //NOTE: it is used for generating indices later
    int baseCenterIndex = (int)m_vertices.size() / 3;
    int topCenterIndex = baseCenterIndex + sectors + 1; // include center vertex

    // put base and top vertices to arrays
    for (int i = 0; i < 2; ++i)
    {
        float h = -height / 2.0f + i * height;              // z value  [-h/2 to h/2]
        float nz = -1 + i * 2;                              // z val normal [-1 to 1]

        // center point
        m_vertices.push_back(0);
        m_vertices.push_back(0);
        m_vertices.push_back(h);
        //normals.push_back(0);      normals.push_back(0);      normals.push_back(nz);
        m_textureCoords.push_back(0.5f); m_textureCoords.push_back(0.5f);

        for (int j = 0, k = 0; j < sectors; ++j, k += 3)
        {
            const float rad = bottomRadius + (float)i / stacks * (topRadius - bottomRadius);     // lerp
            const float ux = unitCircleVertices[k];
            const float uy = unitCircleVertices[k + 1];
            // position
            // m_vertices.push_back(ux * radius);             // vx
            m_vertices.push_back(ux * rad);             // vx
            // m_vertices.push_back(uy * radius);             // vy
            m_vertices.push_back(uy * rad);             // vy
            m_vertices.push_back(h);                       // vz
            // normal
            //normals.push_back(0);                        // nx
            //normals.push_back(0);                        // ny
            //normals.push_back(nz);                       // nz
            // tex
            m_textureCoords.push_back(-ux * 0.5f + 0.5f);      // s
            m_textureCoords.push_back(-uy * 0.5f + 0.5f);      // t
        }
    }


    // indices
    int k1 = 0;                         // 1st vertex index at base
    int k2 = sectors + 1;           // 1st vertex index at top

    // indices for the side surface
    for (int i = 0; i < sectors; ++i, ++k1, ++k2)
    {
        // 2 triangles per sector
        // k1 => k1+1 => k2
        m_indices.push_back(k1);
        m_indices.push_back(k1 + 1);
        m_indices.push_back(k2);

        // k2 => k1+1 => k2+1
        m_indices.push_back(k2);
        m_indices.push_back(k1 + 1);
        m_indices.push_back(k2 + 1);
    }

    // indices for the base
    for (int i = 0, k = baseCenterIndex + 1; i < sectors; ++i, ++k)
    {
        if (i < sectors - 1)
        {
            m_indices.push_back(baseCenterIndex);
            m_indices.push_back(k + 1);
            m_indices.push_back(k);
        }
        else
        {
            // last triangle
            m_indices.push_back(baseCenterIndex);
            m_indices.push_back(baseCenterIndex + 1);
            m_indices.push_back(k);
        }
    }

    // indices for the top 
    for (int i = 0, k = topCenterIndex + 1; i < sectors; ++i, ++k)
    {
        if (i < sectors - 1)
        {
            m_indices.push_back(topCenterIndex);
            m_indices.push_back(k);
            m_indices.push_back(k + 1);
        }
        else
        {
            // last triangle
            m_indices.push_back(topCenterIndex);
            m_indices.push_back(k);
            m_indices.push_back(topCenterIndex + 1);
        }
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
