#pragma once

#include "../drawable.h"
#include "../../../utils/gl_call.h"

class Cube : public Drawable
{
public:
	using Drawable::Drawable;
	Cube(const Shader& shader, const glm::vec3& color);
	Cube(const Shader& shader, const glm::vec3& color, const Texture& texture);
};

inline Cube::Cube(const Shader& shader, const glm::vec3& color) : Drawable(shader, color)
{
	Transform.GuiDisplay = "Cube";
	
	m_vertices = {
		// Front face
		0.5f,  0.5f, 0.5f,
	   -0.5f,  0.5f, 0.5f,
	   -0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
	   // Back face
		0.5f,  0.5f, -0.5f,
	   -0.5f,  0.5f, -0.5f,
	   -0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
   };

	m_indices = {
		// Front
		0, 1, 2,
		2, 3, 0,
		// Right
		0, 3, 7,
		7, 4, 0,
		// Bottom
		2, 6, 7,
		7, 3, 2,
		// Left
		1, 5, 6,
		6, 2, 1,
		// Back
		4, 7, 6,
		6, 5, 4,
		// Top
		5, 1, 0,
		0, 4, 5,
	};

	m_textureCoords = {
		// Front face
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		// Back face
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
	};

	
	// Create the VAO, VBO, and EBO
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	// Bind the VAO
	glBindVertexArray(m_vao);

	// Bind and buffer data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

	// Bind and buffer data into EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

	// Set the vertex attribute pointers for position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind VAO
	glBindVertexArray(0);
}

inline Cube::Cube(const Shader& shader, const glm::vec3& color, const Texture& texture) : Drawable(shader, color, texture)
{
	Transform.GuiDisplay = "Cube";

	m_vertices = {
        // Front face
        0.5f,  0.5f, 0.5f,
       -0.5f,  0.5f, 0.5f,
       -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
       // Back face
        0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
    };

    m_indices = {
        // Front
        0, 1, 2,
        2, 3, 0,
        // Right
        0, 3, 7,
        7, 4, 0,
        // Bottom
        2, 6, 7,
        7, 3, 2,
        // Left
        1, 5, 6,
        6, 2, 1,
        // Back
        4, 7, 6,
        6, 5, 4,
        // Top
        5, 1, 0,
        0, 4, 5,
    };

    m_textureCoords = {
        // Front face
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        // Back face
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
    };

	// Create the VAO, VBO, and EBO
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	// Bind the VAO
	glBindVertexArray(m_vao);

	// Bind and buffer data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

	// Bind and buffer data into EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);
	
	// Set the vertex attribute pointers for position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// Textures
	glGenBuffers(1, &m_textureVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_textureVbo);
	glBufferData(GL_ARRAY_BUFFER, m_textureCoords.size() * sizeof(float), m_textureCoords.data(), GL_STATIC_DRAW);

	// Attribute pointer at 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	// Unbind VAO
	glBindVertexArray(0);
}

