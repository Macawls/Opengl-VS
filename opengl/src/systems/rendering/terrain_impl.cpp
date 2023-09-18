#include "terrain.h"
#include <vector>

#include <GL/glew.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image/stb_image_resize.h"

Terrain::Terrain(const char* heightmapPath, Shader shader, TerrainOptions options)
{
    m_shader = shader;

    unsigned char* data = stbi_load(heightmapPath,
        &m_width, &m_height, &m_nChannels, 0);

    // Resize if necessary
    if (options.desiredWidth != m_width || options.desiredLength != m_height)
    {
        unsigned char* resizedData = new unsigned char[options.desiredWidth * options.desiredLength * m_nChannels];
        stbir_resize_uint8(data, m_width, m_height, 0, resizedData, options.desiredWidth, options.desiredLength, 0, m_nChannels);
        stbi_image_free(data);
        data = resizedData;
        m_width = options.desiredWidth;
        m_height = options.desiredLength;
    }

    // Generate vertices
    std::vector<float> vertices;

    float yScale = options.maxHeight / 256.0f;
    float yShift = 16.0f;
    for (unsigned int i = 0; i < m_height; i++)
    {
        for (unsigned int j = 0; j < m_width; j++)
        {
            // texel for (i,j) tex coord
            unsigned char* texel = data + (j + m_width * i) * m_nChannels;
            // height at coordinate
            unsigned char y = texel[0];

            // z first because winding order is ccw
            // z
            vertices.push_back(-m_width / 2.0f + j);
            // y 
            vertices.push_back((int)y * yScale - yShift);
            // x
            vertices.push_back(-m_height / 2.0f + i);
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

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        &vertices[0],
        GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        &indices[0],
        GL_STATIC_DRAW);
}

void Terrain::Draw(PerspectiveCamera& camera)
{
    const unsigned int NUM_STRIPS = m_height - 1;
    const unsigned int NUM_VERTS_PER_STRIP = m_width * 2;

    m_shader.Use()
        .SetMat4("projection", camera.GetProjectionMatrix())
        .SetMat4("view", camera.GetViewMatrix())
        .SetMat4("model", Transform.GetModelMatrix());

    // draw mesh
    glBindVertexArray(m_VAO);

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
}

