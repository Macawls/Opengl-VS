#include "terrain.h"
#include <vector>

#include <GL/glew.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image/stb_image_resize.h"

Terrain::Terrain(const char* heightmapPath, const ShaderComponent& shader, const TerrainOptions& options) : Drawable(shader)
{
    Transform.GuiDisplay = "Terrain";

    unsigned char* data = stbi_load(heightmapPath, &m_width, &m_height, &m_nChannels, 0);

    // Resize if necessary
    if (options.DesiredWidth != m_width || options.DesiredLength != m_height)
    {
        unsigned char* resizedData = new unsigned char[options.DesiredWidth * options.DesiredLength * m_nChannels];
        stbir_resize_uint8(data, m_width, m_height, 0, resizedData, options.DesiredWidth, options.DesiredLength, 0, m_nChannels);
        stbi_image_free(data);
        data = resizedData;
        m_width = options.DesiredWidth;
        m_height = options.DesiredLength;
    }

    float yScale = options.MaxHeight / 256.0f;
    float yShift = 16.0f;

    m_vertices.clear();

    for (unsigned int i = 0; i < m_height; i++)
    {
        for (unsigned int j = 0; j < m_width; j++)
        {
            unsigned char* texel = data + (j + m_width * i) * m_nChannels;
            unsigned char y = texel[0];

            // Calculate vertex position
            glm::vec3 position = glm::vec3( -m_width / 2.0f + j, static_cast<float>(y) * yScale - yShift, -m_height / 2.0f + i);
            glm::vec3 normal(0.0f);

            /*
            if (i < m_height - 1 && j < m_width - 1) {
                glm::vec3 v1 = vertices[i * m_width + j].Position;
                glm::vec3 v2 = vertices[i * m_width + (j + 1)].Position;
                glm::vec3 v3 = vertices[(i + 1) * m_width + j].Position;

                glm::vec3 edge1 = v2 - v1;
                glm::vec3 edge2 = v3 - v1;

                normal = cross(edge1, edge2);
            }
            */

            // use repeat factor for UV
            float u = static_cast<float>(j) / (m_width - 1) * options.RepeatFactor.factorU;
            float v = static_cast<float>(i) / (m_height - 1) * options.RepeatFactor.factorV;
            auto tex = glm::vec2(u, v);

            // Create a Vertex struct and add it to the vector
            m_vertices.push_back( { position,normalize(normal), tex });
        }
    }

    stbi_image_free(data);

    // Generate indices
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < m_height - 1; i++)  // row
    {
        for (unsigned int j = 0; j < m_width; j++)   // column
        {
            for (unsigned int k = 0; k < 2; k++)   // each side of the strip
            {
                indices.push_back(j + m_width * (i + k));
            }
        }
    }

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
        m_vertices.size() * sizeof(Vertex),
        &m_vertices[0],
        GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);

    // texture coordinates attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        &indices[0],
        GL_STATIC_DRAW);
}

void Terrain::Draw(PerspectiveCamera& camera)
{
    const unsigned int NUM_STRIPS = m_height - 1;
    const unsigned int NUM_VERTS_PER_STRIP = m_width * 2;

    Shader.Use()
        .SetMat4("projection", camera.GetProjectionMatrix())
        .SetMat4("view", camera.GetViewMatrix())
        .SetMat4("model", Transform.GetModelMatrix());

    
    Texture.Bind(0);
    Shader.SetInt("texture1", 0);
    
    // draw mesh
    glBindVertexArray(m_vao);

    // render the mesh triangle strip by triangle strip - each row at a time
    for (unsigned int strip = 0; strip < NUM_STRIPS; ++strip)
    {
        glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
            NUM_VERTS_PER_STRIP, // number of indices to render
            GL_UNSIGNED_INT,     // index data type
            (void*)(sizeof(unsigned int)
                * NUM_VERTS_PER_STRIP
                * strip)); // offset to starting index
    }

    Texture.Unbind();
}


