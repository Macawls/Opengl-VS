#include "cube.h"

Cube::Cube(Shader shader, glm::vec3 color)
{
	m_shader = shader;
    m_color = color;
    init();
}

glm::mat4 Cube::get_mvp(const PerspectiveCamera& camera)
{
	glm::mat4 model = Transform.GetModelMatrix();
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = camera.GetProjectionMatrix();

	return projection * view * model;
}

void Cube::Draw(const PerspectiveCamera& camera)
{
	// Set shader in opengl state machine
	m_shader.Use()
        .SetMat4("mvp", get_mvp(camera))
	    .SetVec3("color", m_color);

	// Draw cube
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Unbind
	glBindVertexArray(0);
}

void Cube::Draw(const PerspectiveCamera& camera, glm::vec3 color)
{
	m_shader.Use()
		.SetMat4("mvp", get_mvp(camera))
		.SetVec3("color", color);

	// Draw cube
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Unbind
	glBindVertexArray(0);
}

void Cube::init()
{
    // Create the VAO, VBO, and EBO
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    // Bind the VAO
    glBindVertexArray(m_vao);

    // Bind and buffer data into VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

    // Bind and buffer data into EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO
    glBindVertexArray(0);
}


