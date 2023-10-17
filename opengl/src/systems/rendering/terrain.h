#pragma once

#include "perspective_camera.h"
#include "../shader/shader.h"

#include "stb_image/stb_image.h"

struct TerrainOptions
{
	int DesiredWidth;
	int DesiredLength;
	float MaxHeight;
};

class Terrain
{
public:
	Terrain(const char* heightmapPath, Shader shader, TerrainOptions options);
	void Draw(PerspectiveCamera& camera);
	TransformComponent Transform = TransformComponent();

private:
	unsigned int m_desiredSize;
	
	int m_width, m_height, m_nChannels;
	Shader m_shader;
	std::vector<float> m_vertices;
	unsigned int m_vao, m_vbo, m_ebo;
	std::vector<float> generate_terrain(const char* heightmapPath);
};

