﻿#pragma once

#include "../drawable.h"

class Cone : public Drawable
{
public:
    using Drawable::Drawable;
    Cone(const ShaderComponent& shader, const glm::vec3& color);
    Cone(const ShaderComponent& shader, const glm::vec3& color, const TextureComponent& texture);
    void Construct();
};

inline Cone::Cone(const ShaderComponent& shader, const glm::vec3& color) : Drawable(shader, color)
{
    Construct();
}

inline Cone::Cone(const ShaderComponent& shader, const glm::vec3& color, const TextureComponent& texture) : Drawable(shader, color, texture)
{
    Construct();
}

inline void Cone::Construct()
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
            glm::vec3 pos(ux * rad, uy * rad, h);
            // normal
            glm::vec3 normal(ux, uy, uz);
            // tex
            glm::vec2 tex(static_cast<float>(j) / static_cast<float>(sectors), t);
            
            Vertices.push_back({pos, normal, tex});
        }
    }

    // the starting index for the base/top surface
    //NOTE: it is used for generating indices later
    int baseCenterIndex = static_cast<int>(Vertices.size()) - (sectors + 1) * 2;
    int topCenterIndex = baseCenterIndex + sectors + 1; // include center vertex

    // put base and top vertices to arrays
    for (int i = 0; i < 2; ++i)
    {
        float h = -height / 2.0f + i * height;              // z value  [-h/2 to h/2]
        float nz = -1 + i * 2;                              // z val normal [-1 to 1]

        // Center point
        glm::vec3 centerPos = {0, 0, h};
        glm::vec3 centerNorm = {0, 0, nz};
        glm::vec2 centerTex = {0.5f, 0.5f};
        
        Vertices.push_back({centerPos, centerNorm, centerTex});

        for (int j = 0, k = 0; j < sectors; ++j, k += 3)
        {
            const float rad = bottomRadius + (float)i / stacks * (topRadius - bottomRadius);     // lerp
            const float ux = unitCircleVertices[k];
            const float uy = unitCircleVertices[k + 1];

            // Position
            glm::vec3 position(ux * rad, uy * rad, h);
            // Normal
            glm::vec3 normal(0, 0, nz);
            // Texture
            glm::vec2 tex(-ux * 0.5f + 0.5f, -uy * 0.5f + 0.5f);
            
            Vertices.push_back({ position, normal, tex });
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
        Indices.push_back(k1);
        Indices.push_back(k1 + 1);
        Indices.push_back(k2);

        // k2 => k1+1 => k2+1
        Indices.push_back(k2);
        Indices.push_back(k1 + 1);
        Indices.push_back(k2 + 1);
    }

    // indices for the base
    for (int i = 0, k = baseCenterIndex + 1; i < sectors; ++i, ++k)
    {
        if (i < sectors - 1)
        {
            Indices.push_back(baseCenterIndex);
            Indices.push_back(k + 1);
            Indices.push_back(k);
        }
        else
        {
            // last triangle
            Indices.push_back(baseCenterIndex);
            Indices.push_back(baseCenterIndex + 1);
            Indices.push_back(k);
        }
    }

    // indices for the top 
    for (int i = 0, k = topCenterIndex + 1; i < sectors; ++i, ++k)
    {
        if (i < sectors - 1)
        {
            Indices.push_back(topCenterIndex);
            Indices.push_back(k);
            Indices.push_back(k + 1);
        }
        else
        {
            // last triangle
            Indices.push_back(topCenterIndex);
            Indices.push_back(k);
            Indices.push_back(topCenterIndex + 1);
        }
    }

    Init();
}



