#pragma once

#include "perspective_camera.h"
#include "../shader/shader.h"

#include "stb_image/stb_image.h"

struct TerrainOptions
{
	int desiredWidth;
	int desiredLength;
	float maxHeight;
};

class Terrain
{
public:
	Terrain(const char* heightmapPath, Shader shader, TerrainOptions options);
	void Draw(PerspectiveCamera& camera);
	TransformComponent Transform = TransformComponent();

private:
	unsigned int desiredSize;
	
	int m_width, m_height, m_nChannels;
	Shader m_shader;
	std::vector<float> vertices;
	unsigned int m_VAO, m_VBO, m_EBO;
	std::vector<float> generate_terrain(const char* heightmapPath);
};

